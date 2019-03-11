#include "PCH.hpp"
#include "Triangle.hpp"

#include "lkCommon/Math/Constants.hpp"

namespace lkRay {
namespace Geometry {

Triangle::Triangle(const std::vector<lkCommon::Math::Vector4>& pts,
                   const lkCommon::Math::Vector4& pos)
    : mPointsRef(pts)
    , mPosRef(pos)
    , mPoints{0, 0, 0}
{
}

Triangle::Triangle(const std::vector<lkCommon::Math::Vector4>& pts,
                   const lkCommon::Math::Vector4& pos,
                   uint32_t a, uint32_t b, uint32_t c)
    : mPointsRef(pts)
    , mPosRef(pos)
    , mPoints{a, b, c}
{
}

bool Triangle::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) const
{
    // MT algorithm
    const lkCommon::Math::Vector4& p0 = mPointsRef[mPoints[0]];
    const lkCommon::Math::Vector4& p1 = mPointsRef[mPoints[1]];
    const lkCommon::Math::Vector4& p2 = mPointsRef[mPoints[2]];

    lkCommon::Math::Vector4 E1 = p2 - p0;
    lkCommon::Math::Vector4 E2 = p1 - p0;
    lkCommon::Math::Vector4 pv(E2.Cross(ray.mDirection));
    float d = E1.Dot(pv);

    if (d < LKCOMMON_EPSILON)
        return false; // backface culling

    float invD = 1.0f / d;

    lkCommon::Math::Vector4 tv(ray.mOrigin - p0 - mPosRef);
    lkCommon::Math::Vector4 qv = E1.Cross(tv);
    float u = tv.Dot(pv) * invD;
    float v = ray.mDirection.Dot(qv) * invD;

    if (u < 0.0f || u > 1.0f ||
        v < 0.0f || u + v > 1.0f)
        return false;

    d = E2.Dot(qv) * invD;
    if (d < LKCOMMON_EPSILON)
        return false;

    distance = d;
    normal = E2.Cross(E1).Normalize();
    return true;
}

AABB Triangle::GetBBox()
{
    AABB result;

    result[AABBPoint::MIN] = lkCommon::Math::Vector4(INFINITY);
    result[AABBPoint::MAX] = lkCommon::Math::Vector4(-INFINITY);

    result[AABBPoint::MIN][3] = 1.0f;
    result[AABBPoint::MAX][3] = 1.0f;

    auto minLambda = [](float& src, const float& b) -> void
    {
        if (b < src)
            src = b;
    };

    auto maxLambda = [](float& src, const float& b) -> void
    {
        if (b > src)
            src = b;
    };

    for (uint32_t i = 0; i < 3; ++i)
    {
        const lkCommon::Math::Vector4& point = mPointsRef[mPoints[i]];

        minLambda(result[AABBPoint::MIN][0], point[0]);
        minLambda(result[AABBPoint::MIN][1], point[1]);
        minLambda(result[AABBPoint::MIN][2], point[2]);

        maxLambda(result[AABBPoint::MAX][0], point[0]);
        maxLambda(result[AABBPoint::MAX][1], point[1]);
        maxLambda(result[AABBPoint::MAX][2], point[2]);

    }

    return result;
}

} // namespace Geometry
} // namespace lkRay
