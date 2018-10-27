#include "PCH.hpp"
#include "Mesh.hpp"
#include <limits>

namespace lkRay {
namespace Geometry {

Mesh::Mesh(const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
           const std::vector<Triangle>& indices)
    : Primitive(pos)
    , mPoints(points)
    , mTriangleIndices(indices)
{

}

bool Mesh::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    bool hit = false;
    float resDist = 0.0f;
    lkCommon::Math::Vector4 resNorm;
    distance = std::numeric_limits<float>::max();

    // A very naive approach, just goes through all the tris at once.
    // TODO optimizations:
    //  - Calculate AABB of mesh and test against it
    //  - When above passes, use a method to optimize the selection
    for (uint32_t i = 0; i < mTriangleIndices.size(); ++i)
    {
        if (mTriangleIndices[i].TestCollision(mPosition, mPoints, ray, resDist, resNorm))
        {
            if (resDist < distance)
            {
                hit = true;
                distance = resDist;
                normal = resNorm;
            }
        }
    }

    return hit;
}

} // namespace Geometry
} // namespace lkRay
