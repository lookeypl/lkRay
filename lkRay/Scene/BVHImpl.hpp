#pragma once

#include "PCH.hpp"
#include "BVH.hpp"

#include <lkCommon/Utils/StaticStack.hpp>

#include <algorithm>
#include <numeric>
#include <queue>

#include "Geometry/Primitive.hpp"


namespace {

const uint32_t STACK_MAX_SIZE = 8192;
const uint32_t BUCKET_COUNT = 12;

template <class T>
struct Ptr
{
    T* p;
    Ptr(T& t): p(&t) {};
    T* operator->() { return p; };
};

template <class T>
struct Ptr<const T>
{
    const T* p;
    Ptr(const T& t): p(&t) {};
    const T* operator->() const { return p; };
};

template <class T>
struct Ptr<std::shared_ptr<T>>
{
    T* p;
    Ptr(std::shared_ptr<T> t): p(t.get()) {};
    T* operator->() { return p; };
};

template <class T>
struct Ptr<const std::shared_ptr<T>>
{
    const T* p;
    Ptr(const std::shared_ptr<T>& t): p(t.get()) {};
    const T* operator->() const { return p; };
};

}


namespace lkRay {
namespace Scene {

template <typename T>
BVH<T>::BVH()
    : mRootNode(nullptr)
    , mObjects()
    , mNodes()
{
}

template <typename T>
BVH<T>::~BVH()
{
}

template <typename T>
BVHObjIdCollection::iterator BVH<T>::FindSplitPoint(BVHObjIdCollection& objIds,
                                                    const BVHNode* currentNode)
{
    using AABBPoint = Geometry::AABBPoint;

    // find longest axis
    float longestAxisLen = 0.0f;
    uint32_t longestAxis = UINT32_MAX; // 0 - X, 1 - Y, 2 - Z
    for (uint32_t i = 0; i < 3; ++i)
    {
        float len = currentNode->bBox[AABBPoint::MAX][i] - currentNode->bBox[AABBPoint::MIN][i];
        if (len > longestAxisLen)
        {
            longestAxisLen = len;
            longestAxis = i;
        }
    }

    // sort according to longest axis
    // TODO here's lots of calcs to be done, especially by GetBBox() in some cases. Speedup by precaching the info.
    std::sort(objIds.begin(), objIds.end(), [this, &longestAxis](const uint32_t& a, const uint32_t& b) {
        return Ptr<T>(mObjects[a])->GetBBox().Centre()[longestAxis] + Ptr<T>(mObjects[a])->GetPosition()[longestAxis] <
            Ptr<T>(mObjects[b])->GetBBox().Centre()[longestAxis] + Ptr<T>(mObjects[b])->GetPosition()[longestAxis];
    });

    // find split point
    BVHObjIdCollection::iterator split;

    struct SAHBucket
    {
        uint32_t count;
        Geometry::AABB bounds;

        SAHBucket()
            : count(0)
            , bounds()
        {}
    } buckets[BUCKET_COUNT];

    lkCommon::Math::Vector4 nodeSize = currentNode->bBox[AABBPoint::MAX] - currentNode->bBox[AABBPoint::MIN];

    // fill our buckets
    for (uint32_t i = 0; i < objIds.size(); ++i)
    {
        const Geometry::AABB& objBox = Ptr<T>(mObjects[objIds[i]])->GetBBox();
        const lkCommon::Math::Vector4& pos = Ptr<T>(mObjects[objIds[i]])->GetPosition();

        float offset = objBox.Centre()[longestAxis] + pos[longestAxis] - currentNode->bBox[AABBPoint::MIN][longestAxis];
        offset /= nodeSize[longestAxis];
        uint32_t bucket = static_cast<uint32_t>(BUCKET_COUNT * offset);

        if (bucket == BUCKET_COUNT)
            bucket = BUCKET_COUNT - 1;

        LKCOMMON_ASSERT(bucket < BUCKET_COUNT, "Achieved a bucket " << bucket << " out of range!");
        buckets[bucket].count++;
        buckets[bucket].bounds.Join(objBox);
    }

    // calculate cost per split between buckets
    float nodeSurface = currentNode->bBox.Surface();
    float cost[BUCKET_COUNT];
    for (uint32_t i = 0; i < BUCKET_COUNT; ++i)
    {
        Geometry::AABB bboxLeft, bboxRight;
        uint32_t countLeft = 0, countRight = 0;

        // left side
        for (uint32_t j = 0; j <= i; ++j)
        {
            bboxLeft.Join(buckets[j].bounds);
            countLeft += buckets[j].count;
        }

        // right side
        for (uint32_t j = i + 1; j < BUCKET_COUNT; ++j)
        {
            bboxRight.Join(buckets[j].bounds);
            countRight += buckets[j].count;
        }

        // cost includes:
        //   - traversal cost (1)
        //   - surface of left side AABB * count of left elements / surface of node AABB
        //   - surface of right side AABB * count of right elements / surface of node AABB
        cost[i] = 1.0f +
            (bboxLeft.Surface() * countLeft +
             bboxRight.Surface() * countRight) / nodeSurface;
    }

    // find lowest costing split point
    uint32_t lowestCostId = 0;
    float lowestCost = cost[0];
    for (uint32_t i = 1; i < BUCKET_COUNT; ++i)
    {
        if (cost[i] < lowestCost)
        {
            lowestCostId = i;
            lowestCost = cost[i];
        }
    }

    LOGD("For " << objIds.size() << " objects lowest cost is bucket #" << lowestCostId << ": " << lowestCost);

    // advance split operator by summed count items
    uint32_t toAdvance = 0;
    for (uint32_t i = 0; i <= lowestCostId; ++i)
    {
        toAdvance += buckets[i].count;
    }

    split = objIds.begin();
    std::advance(split, toAdvance);

    // in case SAH method fails, just divide the collection into two halves
    // and continue
    if (split == objIds.begin() || split == objIds.end())
    {
        LOGD("Split point bad - falling back to split by halves!");
        split = objIds.begin();
        std::advance(split, objIds.size() / 2);
    }

    return split;
}

template <typename T>
void BVH<T>::BuildStep(BVHObjIdCollection& objIds, BVHNode *currentNode)
{
    using AABBPoint = Geometry::AABBPoint;

    LKCOMMON_ASSERT(objIds.size() != 0, "BVH build error - step has no objects!");

    // debug
    std::stringstream objstr;
    for (const auto& o: objIds)
        objstr << std::to_string(o) << ", ";
    LOGD("Objects in node: " << objstr.str().c_str());

    mNodeCount++;

    // initialize bbox
    currentNode->bBox = Geometry::AABB();

    // calculate group BBox
    for (auto& o: objIds)
    {
        Geometry::AABB objBox = Ptr<T>(mObjects[o])->GetBBox();
        objBox[AABBPoint::MIN] += Ptr<T>(mObjects[o])->GetPosition();
        objBox[AABBPoint::MAX] += Ptr<T>(mObjects[o])->GetPosition();

        currentNode->bBox.Join(objBox);
    }

    // if only two objects, calculate size, set as leaf and return
    if (objIds.size() <= 2)
    {
        // zero midNode data
        currentNode->midData.left = currentNode->midData.right = nullptr;

        // we shouldn't have a node with 0 elements
        currentNode->leafData.obj[0] = objIds.front();

        if (objIds.size() == 2)
            currentNode->leafData.obj[1] = objIds.back();
        else
            currentNode->leafData.obj[1] = UINT32_MAX;

        return;
    }

    auto split = FindSplitPoint(objIds, currentNode);

    // build sub nodes
    BVHObjIdCollection subObjs(objIds.begin(), split);
    if (subObjs.size() > 0)
    {
        mNodes.emplace_back();
        BVHNode* node = &mNodes.back();
        currentNode->midData.left = node;

        BuildStep(subObjs, node);
    }
    else
    {
        currentNode->midData.left = nullptr;
    }

    subObjs.assign(split, objIds.end());
    if (subObjs.size() > 0)
    {
        mNodes.emplace_back();
        BVHNode* node = &mNodes.back();
        currentNode->midData.right = node;

        BuildStep(subObjs, node);
    }
    else
    {
        currentNode->midData.right = nullptr;
    }
}

template <typename T>
void BVH<T>::PrintStep(BVHNode* currentNode, uint32_t depth) const
{
    using AABBPoint = Geometry::AABBPoint;

    if (depth > 3)
        return;

    std::stringstream p;
    for (uint32_t i = 0; i < depth; ++i)
        p << "  ";

    if (currentNode->midData.left == nullptr && currentNode->midData.right == nullptr)
    {
        p << " -> Leaf "
          << currentNode->bBox[AABBPoint::MIN] << " "
          << currentNode->bBox[AABBPoint::MAX] << ": objs "
          << currentNode->leafData.obj[0] << ", "
          << currentNode->leafData.obj[1];
    }
    else
    {
        p << " -> Mid "
          << currentNode->bBox[AABBPoint::MIN] << " "
          << currentNode->bBox[AABBPoint::MAX] << ": nodes "
          << currentNode->midData.left << ", "
          << currentNode->midData.right;
    }

    LOGD(p.str());

    if (currentNode->midData.left)
        PrintStep(currentNode->midData.left, depth + 1);

    if (currentNode->midData.right)
        PrintStep(currentNode->midData.right, depth + 1);
}

template <typename T>
void BVH<T>::Build()
{
    if (mRootNode)
    {
        // clean existing BVH
        mNodes.clear();
        mRootNode = nullptr;
    }

    LOGD("Building BVH for " << mObjects.size() << " objects");
    mNodeCount = 0;

    std::vector<uint32_t> objIds(mObjects.size(), 0);
    std::iota(objIds.begin(), objIds.end(), 0);

    mNodes.emplace_back();
    mRootNode = &mNodes.back();
    BuildStep(objIds, mRootNode);

    LOGI("BVH built for " << mObjects.size() << " objects");
}

template <typename T>
void BVH<T>::Clean()
{
    mRootNode = nullptr;
    mNodes.clear();
    mObjects.clear();
}

template <typename T>
Renderer::RayCollision BVH<T>::Traverse(const Geometry::Ray& ray) const
{
    BVHNode* node = nullptr;
    float tmin = 0.0f, tmax = 0.0f;
    Renderer::RayCollision result;

    lkCommon::Math::Vector4 rayDirInv(
        1.0f / ray.mDirection[0],
        1.0f / ray.mDirection[1],
        1.0f / ray.mDirection[2],
        0.0f
    );
    int rayDirSign[3] = {
        (rayDirInv[0] < 0.0f),
        (rayDirInv[1] < 0.0f),
        (rayDirInv[2] < 0.0f)
    };

    result.mDistance = INFINITY;

    if (!mRootNode->bBox.TestCollision(ray, rayDirInv, rayDirSign, tmin, tmax))
        return result;

    LKCOMMON_ASSERT(mNodeCount <= STACK_MAX_SIZE, "STACK NOT BIG ENOUGH");
    lkCommon::Utils::StaticStack<BVHNode*, STACK_MAX_SIZE> stack;
    stack.Emplace(mRootNode);

    while (stack.Size())
    {
        BVHNode* node = stack.Pop();
        LKCOMMON_ASSERT(node != nullptr, "Received node from stack which is NULL.");

        // ray collides with bbox, check if we are a leaf node
        if (node->midData.left == nullptr && node->midData.right == nullptr)
        {
            // we are a leaf node, test intersection with both objects
            struct _res {
                bool c;
                int32_t id;
                float d;
                lkCommon::Math::Vector4 n;
                Geometry::UV uv;

                _res()
                    : c(false)
                    , id(-1)
                    , d(INFINITY)
                    , n()
                    , uv()
                {
                }
            } res[2];

            res[0].c =
                Ptr<const T>(mObjects[node->leafData.obj[0]])->TestCollision(ray, res[0].d, res[0].n, res[0].uv);
            if (res[0].c)
                res[0].id = node->leafData.obj[0];

            if (node->leafData.obj[1] != UINT32_MAX)
            {
                res[1].c =
                    Ptr<const T>(mObjects[node->leafData.obj[1]])->TestCollision(ray, res[1].d, res[1].n, res[1].uv);
                if (res[1].c)
                    res[1].id = node->leafData.obj[1];
            }

            if (res[0].c || res[1].c)
            {
                if (res[1].d < res[0].d)
                    std::swap(res[0], res[1]);

                if (res[0].d < result.mDistance)
                {
                    result.mHitID = res[0].id;
                    result.mDistance = res[0].d;
                    result.mNormal = res[0].n;
                    result.mUV = res[0].uv;
                }
            }

            continue;
        }

        // not a leaf node, traverse further
        if (node->midData.left->bBox.TestCollision(ray, rayDirInv, rayDirSign, tmin, tmax))
            stack.Emplace(node->midData.left);

        if (node->midData.right->bBox.TestCollision(ray, rayDirInv, rayDirSign, tmin, tmax))
            stack.Emplace(node->midData.right);
    }

    result.mPoint = ray.mOrigin + ray.mDirection * result.mDistance;
    return result;
}

template <typename T>
void BVH<T>::Print() const
{
    PrintStep(mRootNode, 0);
}

} // namespace Scene
} // namespace lkRay
