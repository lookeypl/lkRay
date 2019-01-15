#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"
#include "AABB.hpp"

#include "Types.hpp"
#include "Scene/Containers.hpp"

#include <memory>
#include <rapidjson/document.h>


namespace lkRay {
namespace Geometry {

class Primitive
{
protected:
    std::string mName;
    lkCommon::Math::Vector4 mPosition;
    Material::Material* mMaterial;
    AABB mBBox;

    bool ReadPositionFromNode(const rapidjson::Value& value);
    bool ReadMaterialFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials);

public:
    Primitive(const std::string& name);
    Primitive(const std::string& name, const lkCommon::Math::Vector4& position);
    ~Primitive() = default;

    // returns true if ray collided with something
    // then, returns through arguments closest positive distance to ray origin and surface normal at collision point
    // false if there's no collision (ray missed the sphere), then both distance and normal are unmodified
    virtual bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) = 0;
    virtual void CalculateBBox() = 0;
    virtual bool ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials) = 0;
    virtual Types::Primitive GetType() const = 0;


    LKCOMMON_INLINE void SetPosition(const lkCommon::Math::Vector4& pos)
    {
        mPosition = pos;
    }

    LKCOMMON_INLINE void SetMaterial(Material::Material* material)
    {
        mMaterial = material;
    }


    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }

    LKCOMMON_INLINE Material::Material* GetMaterial() const
    {
        return mMaterial;
    }

    LKCOMMON_INLINE const AABB& GetBBox() const
    {
        return mBBox;
    }
};

} // namespace Geometry
} // namespace lkRay
