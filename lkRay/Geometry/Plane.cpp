#include "PCH.hpp"
#include "Plane.hpp"

namespace lkRay {
namespace Geometry {

Plane::Plane(const lkCommon::Math::Vector4& normal, float distance)
    : mNormal(normal)
    , mD(distance)
{
}

bool Plane::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    // Geometric approach to finding a triangle:
    //   1. Check if ray hits Triangle's plane
    //   2. Check if intersection is inside triangle (inside-outside method)
    // TODO replace with MT algorithm

    float normalDotDir = mNormal.Dot(ray.GetDirection());
    if (normalDotDir > 0)
        return false;

    distance = -(mNormal.Dot(ray.GetOrigin()) - mD) / normalDotDir;
    if (distance < 0)
        return false;

    normal = mNormal;
    return true;
}

} // namespace Geometry
} // namespace lkRay
