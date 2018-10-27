#include "PCH.hpp"
#include "Sphere.hpp"
#include <cmath>


namespace lkRay {
namespace Geometry {

Sphere::Sphere(const lkCommon::Math::Vector4& origin, float r)
    : Primitive(origin)
    , mRadiusSquare(r*r)
{
}

bool Sphere::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    lkCommon::Math::Vector4 L = mPosition - ray.mOrigin;
    float midPoint = L.Dot(ray.mDirection);
    if (midPoint < EPSILON)
        return false; // sphere is behind us

    float distFromOriginSquare = L.Dot(L) - midPoint * midPoint;
    if (distFromOriginSquare < EPSILON)
        return false; // wut
    if (distFromOriginSquare > mRadiusSquare)
        return false; // missed sphere

    float midToIntersection = sqrtf(mRadiusSquare - distFromOriginSquare);
    float s0 = midPoint - midToIntersection;
    float s1 = midPoint + midToIntersection;

    distance = s0 < s1 ? s0 : s1;

    lkCommon::Math::Vector4 collisionPoint = ray.mOrigin + ray.mDirection * distance;
    normal = (collisionPoint - mPosition).Normalize();

    return true;
}

} // namespace Geometry
} // namespace lkRay
