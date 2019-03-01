#pragma once

#include "PCH.hpp"
#include "BVH.hpp"

#include <lkCommon/Utils/Sort.hpp>

#include <algorithm>
#include <numeric>
#include <queue>

#include "Geometry/Primitive.hpp"


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
bool BVH<T>::UpdateNodeAABB(Geometry::AABB& bbox, uint32_t objID)
{
    using AABBPoint = Geometry::AABBPoint;

    // like std::min, but additionally sets a "modified" flag to false.
    auto minWithFlag = [](float& src, const float& b, bool& modified)
    {
        if (b <= src)
            src = b;
        else
            modified = false;
    };

    auto maxWithFlag = [](float& src, const float& b, bool& modified) -> void
    {
        if (b >= src)
            src = b;
        else
            modified = false;
    };

    bool allModified = true;

    // get bbox and shift it with position
    Geometry::AABB objBox = mObjects[objID]->GetBBox();
    objBox[AABBPoint::MIN] += mObjects[objID]->GetPosition();
    objBox[AABBPoint::MAX] += mObjects[objID]->GetPosition();

    minWithFlag(bbox[AABBPoint::MIN][0], objBox[AABBPoint::MIN][0], allModified);
    minWithFlag(bbox[AABBPoint::MIN][1], objBox[AABBPoint::MIN][1], allModified);
    minWithFlag(bbox[AABBPoint::MIN][2], objBox[AABBPoint::MIN][2], allModified);

    maxWithFlag(bbox[AABBPoint::MAX][0], objBox[AABBPoint::MAX][0], allModified);
    maxWithFlag(bbox[AABBPoint::MAX][1], objBox[AABBPoint::MAX][1], allModified);
    maxWithFlag(bbox[AABBPoint::MAX][2], objBox[AABBPoint::MAX][2], allModified);

    return allModified;
}

template <typename T>
void BVH<T>::BuildStep(std::vector<uint32_t>& objIds, BVHNode *currentNode)
{
    using AABBPoint = Geometry::AABBPoint;

    // debug
    std::stringstream objstr;
    for (const auto& o: objIds)
        objstr << std::to_string(o) << ", ";
    LOGD("Objects in node: " << objstr.str().c_str());

    mNodeCount++;

    // initialize bbox
    currentNode->bBox[AABBPoint::MIN] = lkCommon::Math::Vector4(INFINITY);
    currentNode->bBox[AABBPoint::MAX] = lkCommon::Math::Vector4(-INFINITY);

    currentNode->bBox[AABBPoint::MIN][3] = 1.0f;
    currentNode->bBox[AABBPoint::MAX][3] = 1.0f;

    // calculate group BBox
    for (auto& o: objIds)
        UpdateNodeAABB(currentNode->bBox, o);

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
            currentNode->leafData.obj[1] = std::numeric_limits<uint32_t>::max();

        return;
    }

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
    std::sort(objIds.begin(), objIds.end(), [this, &longestAxis](const uint32_t& a, const uint32_t& b) {
        return mObjects[a]->GetPosition()[longestAxis] < mObjects[b]->GetPosition()[longestAxis];
    });

    // find split point
    // TODO use SAH instead of midpoints
    float midpoint = (currentNode->bBox[AABBPoint::MIN][longestAxis] + currentNode->bBox[AABBPoint::MAX][longestAxis]) / 2.0f;

    // find iterator for split node
    auto split = std::find_if(objIds.begin(), objIds.end(),
    [this, &midpoint, &longestAxis](const uint32_t& objId) {
        if (mObjects[objId]->GetType() == Types::Primitive::PLANE)
            return false;

        return mObjects[objId]->GetPosition()[longestAxis] > midpoint;
    });

    // in case splitting by mid point fails, just divide the collection into two halves
    // and continue
    if (split == objIds.begin() || split == objIds.end())
    {
        split = objIds.begin();
        std::advance(split, objIds.size() / 2);
    }

    // build left node
    std::vector<uint32_t> subObjs(objIds.begin(), split);
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
}

template <typename T>
void BVH<T>::Clean()
{
    mRootNode = nullptr;
    mNodes.clear();
    mObjects.clear();
}

template <typename T>
int32_t BVH<T>::Traverse(const Geometry::Ray& ray,
                         float& distance,
                         lkCommon::Math::Vector4& normal) const
{
    BVHNode* node = nullptr;
    float tmin = 0.0f, tmax = 0.0f;
    int32_t objIDResult = -1;

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

    distance = INFINITY;

    if (!mRootNode->bBox.TestCollision(ray, rayDirInv, rayDirSign, tmin, tmax))
        return objIDResult;

    // queue element = tmin collision distance + ptr to node
    using StackElement = std::pair<float, BVHNode*>;
    std::vector<StackElement> heap;
    heap.reserve(mNodeCount);

    heap.emplace_back(std::make_pair(0.0f, mRootNode));
    auto compare = [](const StackElement& a, const StackElement& b) -> bool
    {
        return a.first > b.first;
    };

    while (!heap.empty())
    {
        std::pop_heap(heap.begin(), heap.end(), compare);
        BVHNode* node = heap.back().second;
        heap.pop_back();

        LKCOMMON_ASSERT(node != nullptr, "Received node from heap which is NULL.");

        // ray collides with bbox, check if we are a leaf node
        if (node->midData.left == nullptr && node->midData.right == nullptr)
        {
            bool collided = false;
            struct _res {
                int32_t id;
                float d;
                lkCommon::Math::Vector4 n;

                _res()
                    : id(-1)
                    , d(INFINITY)
                    , n()
                {
                }
            } res[2];

            // we are a leaf node, test intersection with both objects and leave
            collided = mObjects[node->leafData.obj[0]]->TestCollision(ray, res[0].d, res[0].n);
            if (collided)
                res[0].id = node->leafData.obj[0];

            if (node->leafData.obj[1] != UINT32_MAX)
            {
                collided |= mObjects[node->leafData.obj[1]]->TestCollision(ray, res[1].d, res[1].n);
                if (collided)
                    res[1].id = node->leafData.obj[1];
            }

            if (collided)
            {
                if (res[1].d < res[0].d)
                    std::swap(res[0], res[1]);

                if (res[0].d < distance)
                {
                    objIDResult = res[0].id;
                    distance = res[0].d;
                    normal = res[0].n;
                }
            }

            continue;
        }

        // not a leaf node, traverse further
        if (node->midData.left &&
            node->midData.left->bBox.TestCollision(ray, rayDirInv, rayDirSign, tmin, tmax))
        {
            heap.emplace_back(std::make_pair(tmin, node->midData.left));
            std::push_heap(heap.begin(), heap.end(), compare);
        }

        if (node->midData.right &&
            node->midData.right->bBox.TestCollision(ray, rayDirInv, rayDirSign, tmin, tmax))
        {
            heap.emplace_back(std::make_pair(tmin, node->midData.right));
            std::push_heap(heap.begin(), heap.end(), compare);
        }
    }

    return objIDResult;
}

template <typename T>
void BVH<T>::Print() const
{
    PrintStep(mRootNode, 0);
}

} // namespace Scene
} // namespace lkRay
