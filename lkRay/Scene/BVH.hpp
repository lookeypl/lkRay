#pragma once

#include "Geometry/AABB.hpp"

#include "Containers.hpp"

#include <list>
#include <functional>


namespace lkRay {
namespace Scene {

struct BVHNode;

struct BVHLeafData
{
    uint32_t obj[2];

    BVHLeafData()
        : obj{ UINT32_MAX, UINT32_MAX }
    {
    }
};

struct BVHMidData
{
    BVHNode* left;
    BVHNode* right;

    BVHMidData()
        : left(nullptr)
        , right(nullptr)
    {
    }
};

struct BVHNode
{
    Geometry::AABB bBox;
    BVHMidData midData;
    BVHLeafData leafData;
};

using BVHObjIdCollection = std::vector<uint32_t>;

template <typename T>
class BVH
{
    BVHNode* mRootNode;
    Containers::Container<T> mObjects;
    std::list<BVHNode> mNodes;
    size_t mNodeCount;

    void UpdateNodeAABB(Geometry::AABB& bbox, uint32_t objID);
    BVHObjIdCollection::iterator FindSplitPoint(BVHObjIdCollection& ids, const BVHNode* currentNode);
    void BuildStep(BVHObjIdCollection& objIds, BVHNode* currentNode);
    void PrintStep(BVHNode* currentNode, uint32_t depth) const;

public:
    BVH();
    ~BVH();

    void Build();
    void Clean();
    int32_t Traverse(const Geometry::Ray& ray, float& distance, lkCommon::Math::Vector4& normal) const;
    void Print() const;

    LKCOMMON_INLINE void PushObject(T& ptr)
    {
        mObjects.push_back(ptr);
    }

    template <typename... Ts>
    LKCOMMON_INLINE void EmplaceObject(Ts&&... args)
    {
        mObjects.emplace_back(std::forward<Ts>(args)...);
    }

    LKCOMMON_INLINE const Containers::Primitive& GetPrimitives() const
    {
        return mObjects;
    }

    LKCOMMON_INLINE const T& GetObject(size_t i) const
    {
        return mObjects[i];
    }

    LKCOMMON_INLINE size_t GetObjectCount() const
    {
        return mObjects.size();
    }
};

} // namespace Scene
} // namespace lkRay

#include "BVHImpl.hpp"
