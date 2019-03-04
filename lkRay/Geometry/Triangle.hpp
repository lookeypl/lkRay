#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"
#include "AABB.hpp"

#include <memory>


namespace lkRay {
namespace Geometry {

struct UV
{
    float u;
    float v;

    UV()
        : u(0.0f)
        , v(0.0f)
    {
    }

    UV(float U, float V)
        : u(U)
        , v(V)
    {
    }
};

// Triangle does not inherit Primitive, as it's a building block for Model
class Triangle
{
public:
    using Ptr = std::shared_ptr<Triangle>;

private:
    const std::vector<lkCommon::Math::Vector4>& mPointsRef;
    const lkCommon::Math::Vector4& mPosRef;
    uint32_t mPoints[3];

public:
    Triangle(const std::vector<lkCommon::Math::Vector4>& pts, const lkCommon::Math::Vector4& pos);
    Triangle(const std::vector<lkCommon::Math::Vector4>& pts, const lkCommon::Math::Vector4& pos,
             uint32_t a, uint32_t b, uint32_t c);
    ~Triangle() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) const;
    AABB GetBBox();

    LKCOMMON_INLINE uint32_t& operator[](const size_t i)
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
