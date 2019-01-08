#include "PCH.hpp"
#include "Triangle.hpp"

#include "lkCommon/Math/Constants.hpp"

namespace lkRay {
namespace Geometry {

Triangle::Triangle()
    : mPoints{0, 0, 0}
{
}

Triangle::Triangle(uint32_t a, uint32_t b, uint32_t c)
    : mPoints{a, b, c}
{
}

bool Triangle::TestCollision(const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
                             const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    // MT algorithm
    const lkCommon::Math::Vector4& p0 = points[mPoints[0]];
    const lkCommon::Math::Vector4& p1 = points[mPoints[1]];
    const lkCommon::Math::Vector4& p2 = points[mPoints[2]];

    lkCommon::Math::Vector4 E1 = p1 - p0;
    lkCommon::Math::Vector4 E2 = p2 - p0;
    lkCommon::Math::Vector4 pv(E2.Cross(ray.mDirection));
    float d = E1.Dot(pv);

    if (-LKCOMMON_EPSILON < d && d < LKCOMMON_EPSILON)
        return false; // backface culling

    float invD = 1.0f / d;

    lkCommon::Math::Vector4 tv(ray.mOrigin - p0 - pos);
    lkCommon::Math::Vector4 qv = E1.Cross(tv);
    float u = tv.Dot(pv) * invD;
    float v = ray.mDirection.Dot(qv) * invD;

    if (u < 0.0f || u > 1.0f ||
        v < 0.0f || u + v > 1.0f)
        return false;

    distance = E2.Dot(qv) * invD;
    if (distance < LKCOMMON_EPSILON)
        return false;

    normal = E2.Cross(E1).Normalize();
    return true;
}

} // namespace Geometry
} // namespace lkRay
