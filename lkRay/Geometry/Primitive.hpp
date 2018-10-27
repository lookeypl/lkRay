#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"
#include "Scene/Material.hpp"

#include <memory>


namespace lkRay {
namespace Geometry {

class Primitive
{
public:
    using Ptr = std::shared_ptr<Primitive>;

    enum class Type: unsigned char
    {
        Unknown = 0,
        Sphere,
    };

protected:
    lkCommon::Math::Vector4 mPosition;
    Scene::Material mMaterial;

public:
    Primitive();
    Primitive(const lkCommon::Math::Vector4& position);
    ~Primitive() = default;

    // returns true if ray collided with something
    // then, returns through arguments closest positive distance to ray origin and surface normal at collision point
    // false if there's no collision (ray missed the sphere), then both distance and normal are unmodified
    virtual bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) = 0;


    LKCOMMON_INLINE void SetPosition(const lkCommon::Math::Vector4& pos)
    {
        mPosition = pos;
    }

    LKCOMMON_INLINE void SetMaterial(const Scene::Material& material)
    {
        mMaterial = material;
    }


    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }

    LKCOMMON_INLINE const Scene::Material& GetMaterial() const
    {
        return mMaterial;
    }
};

} // namespace Geometry
} // namespace lkRay
