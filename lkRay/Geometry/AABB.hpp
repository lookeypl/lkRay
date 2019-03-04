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

    lkCommon::Math::Vector4& operator[](const AABBPoint p);
    const lkCommon::Math::Vector4& operator[](const AABBPoint p) const;

    bool operator== (const AABB& other) const;

    // AABB is more of an internal element rather than an actual scene primitive
    // thus, TestCollision definition is a bit different for faster intersection tests
    bool TestCollision(const Geometry::Ray& ray,
                       const lkCommon::Math::Vector4& rayDirInv,
                       const int rayDirSign[3],
                       float& tmin, float& tmax) const;
};

} // namespace Geometry
} // namespace lkRay
