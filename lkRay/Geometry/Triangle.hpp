#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"
#include "AABB.hpp"
#include "UV.hpp"
#include "Vertex.hpp"

#include <memory>


namespace lkRay {
namespace Geometry {

// Triangle does not inherit Primitive, as it's a building block for Model
class Triangle
{
    const Vertices& mPointsRef;
    const lkCommon::Math::Vector4& mPosRef;
    uint32_t mPoints[3];

public:
    Triangle(const Vertices& pts, const lkCommon::Math::Vector4& pos);
    Triangle(const Vertices& pts, const lkCommon::Math::Vector4& pos,
             uint32_t a, uint32_t b, uint32_t c);
    ~Triangle() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal, UV& uv) const;
    AABB GetBBox() const;

    LKCOMMON_INLINE uint32_t& operator[](const size_t i)
    {
        LKCOMMON_ASSERT(i < 3, "Invalid index - triangles only have 3 indices");
        return mPoints[i];
    }

    LKCOMMON_INLINE const uint32_t& operator[](const size_t i) const
    {
        LKCOMMON_ASSERT(i < 3, "Invalid index - triangles only have 3 indices");
        return mPoints[i];
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosRef;
    }
};

} // namespace Geometry
} // namespace lkRay
