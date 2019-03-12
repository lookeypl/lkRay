#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"

namespace lkRay {
namespace Geometry {

enum class AABBPoint: unsigned char
{
    MIN = 0,
    MAX,
};

class AABB
{
    lkCommon::Math::Vector4 mPoints[2];

public:
    AABB();
    AABB(const lkCommon::Math::Vector4& min, const lkCommon::Math::Vector4& max);
    ~AABB() = default;

    // Joins two AABB's together, which results in one large one, including this and other.
    void Join(const AABB& other);

    // returns centre point of this AABB
    lkCommon::Math::Vector4 Centre() const;

    // returns surface area of this AABB
    float Surface() const;

    // AABB is more of an internal element rather than an actual scene primitive
    // thus, TestCollision definition is a bit different for faster intersection tests
    bool TestCollision(const Geometry::Ray& ray,
                       const lkCommon::Math::Vector4& rayDirInv,
                       const int rayDirSign[3],
                       float& tmin, float& tmax) const;

    LKCOMMON_INLINE bool operator== (const AABB& other) const
    {
        return (mPoints[0] == other.mPoints[0]) && (mPoints[1] == other.mPoints[1]);
    }

    LKCOMMON_INLINE lkCommon::Math::Vector4& operator[](const AABBPoint p)
    {
        return mPoints[static_cast<std::underlying_type<AABBPoint>::type>(p)];
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& operator[](const AABBPoint p) const
    {
        return mPoints[static_cast<std::underlying_type<AABBPoint>::type>(p)];
    }
};

} // namespace Geometry
} // namespace lkRay
