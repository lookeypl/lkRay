#include "PCH.hpp"
#include "AABB.hpp"

#include <algorithm>


namespace lkRay {
namespace Geometry {

AABB::AABB()
    : mPoints()
{
}

AABB::AABB(const lkCommon::Math::Vector4& min, const lkCommon::Math::Vector4& max)
    : mPoints{min, max}
{
}

lkCommon::Math::Vector4& AABB::operator[](const AABBPoint p)
{
    return mPoints[static_cast<std::underlying_type<AABBPoint>::type>(p)];
}

const lkCommon::Math::Vector4& AABB::operator[](const AABBPoint p) const
{
    return mPoints[static_cast<std::underlying_type<AABBPoint>::type>(p)];
}

bool AABB::operator== (const AABB& other) const
{
    return (mPoints[0] == other.mPoints[0]) && (mPoints[1] == other.mPoints[1]);
}

bool AABB::TestCollision(const Geometry::Ray& ray,
                         const lkCommon::Math::Vector4& rayDirInv,
                         const int rayDirSign[3],
                         float& tmin, float& tmax)
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
