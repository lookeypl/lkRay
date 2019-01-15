#pragma once

#include "Geometry/AABB.hpp"

#include "Containers.hpp"

#include <list>


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

class BVH
{
    BVHNode* mRootNode;
    Containers::Primitive* mObjectsPtr;
    std::list<BVHNode> mNodes;
    size_t mNodeCount;

    bool UpdateNodeAABB(Geometry::AABB& bbox, uint32_t objID);
    void BuildStep(std::vector<uint32_t>& objIds, BVHNode* currentNode);
    void PrintStep(BVHNode* currentNode, uint32_t depth) const;

public:
    BVH();
    ~BVH();

    void Build(Containers::Primitive* objects);
    int32_t Traverse(const Geometry::Ray& ray, float& distance, lkCommon::Math::Vector4& normal) const;
    void Print() const;
};

} // namespace Scene
} // namespace lkRay
