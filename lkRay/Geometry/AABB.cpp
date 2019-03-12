#include "PCH.hpp"
#include "AABB.hpp"

#include <algorithm>


namespace lkRay {
namespace Geometry {

AABB::AABB()
    : mPoints {
        lkCommon::Math::Vector4(INFINITY, INFINITY, INFINITY, INFINITY),
        lkCommon::Math::Vector4(-INFINITY, -INFINITY, -INFINITY, -INFINITY),
    }
{
}

AABB::AABB(const lkCommon::Math::Vector4& min, const lkCommon::Math::Vector4& max)
    : mPoints{min, max}
{
}

void AABB::Join(const AABB& other)
{
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

    minLambda(mPoints[0][0], other[AABBPoint::MIN][0]);
    minLambda(mPoints[0][1], other[AABBPoint::MIN][1]);
    minLambda(mPoints[0][2], other[AABBPoint::MIN][2]);

    maxLambda(mPoints[1][0], other[AABBPoint::MAX][0]);
    maxLambda(mPoints[1][1], other[AABBPoint::MAX][1]);
    maxLambda(mPoints[1][2], other[AABBPoint::MAX][2]);
}

lkCommon::Math::Vector4 AABB::Centre() const
{
    return (mPoints[0] + mPoints[1]) * 0.5f;
}

float AABB::Surface() const
{
    if (mPoints[1][0] < mPoints[0][0] ||
        mPoints[1][1] < mPoints[0][1] ||
        mPoints[1][2] < mPoints[0][2])
        return 0.0f;

    const float x = mPoints[1][0] - mPoints[0][0];
    const float y = mPoints[1][1] - mPoints[0][1];
    const float z = mPoints[1][2] - mPoints[0][2];
    return 2.0f * ((x * y) + (y * z) + (x * z));
}

bool AABB::TestCollision(const Geometry::Ray& ray,
                         const lkCommon::Math::Vector4& rayDirInv,
                         const int rayDirSign[3],
                         float& tmin, float& tmax) const
{
    tmin = (mPoints[    rayDirSign[0]][0] - ray.mOrigin[0]) * rayDirInv[0];
    tmax = (mPoints[1 - rayDirSign[0]][0] - ray.mOrigin[0]) * rayDirInv[0];

    const float tymin = (mPoints[    rayDirSign[1]][1] - ray.mOrigin[1]) * rayDirInv[1];
    const float tymax = (mPoints[1 - rayDirSign[1]][1] - ray.mOrigin[1]) * rayDirInv[1];

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin < tmin)
        tmin = tymin;
    if (tymax > tmax)
        tmax = tymax;

    const float tzmin = (mPoints[    rayDirSign[2]][2] - ray.mOrigin[2]) * rayDirInv[2];
    const float tzmax = (mPoints[1 - rayDirSign[2]][2] - ray.mOrigin[2]) * rayDirInv[2];

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin < tmin)
        tmin = tzmin;
    if (tzmax > tmax)
        tmax = tzmax;

    return (tmin <= tmax);
}

} // namespace Geometry
} // namespace lkRay
