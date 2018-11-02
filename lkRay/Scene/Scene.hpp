#pragma once

#include "Prerequisites.hpp"

#include <string>
#include <vector>
#include "Scene/Light.hpp"
#include "Scene/RayCollision.hpp"
#include "Geometry/Ray.hpp"


namespace lkRay {
namespace Scene {

class Scene final
{
public:
    template <typename T>
    using Container = std::vector<std::shared_ptr<T>>;
    using PrimitiveContainer = Container<Geometry::Primitive>;
    using LightContainer = Container<Light>;

private:
    PrimitiveContainer mPrimitives; // TODO create Scene::Object
    LightContainer mLights;
    lkCommon::Utils::PixelFloat4 mAmbient;

public:
    Scene();
    ~Scene();

    bool Load(const std::string& path);

    void AddPrimitive(const std::shared_ptr<Geometry::Primitive>& primitive);
    void AddLight(const Light::Ptr& light);
    lkCommon::Utils::PixelFloat4 SampleLights(const RayCollision& collision) const;
    RayCollision TestCollision(const Geometry::Ray& ray, int skipObjID) const;


    LKCOMMON_INLINE void SetAmbient(const lkCommon::Utils::PixelFloat4 ambient)
    {
        mAmbient = ambient;
    }


    LKCOMMON_INLINE const PrimitiveContainer& GetPrimitives() const
    {
        return mPrimitives;
    }

    LKCOMMON_INLINE const LightContainer& GetLights() const
    {
        return mLights;
    }

    LKCOMMON_INLINE const lkCommon::Utils::PixelFloat4& GetAmbient() const
    {
        return mAmbient;
    }
};

} // namespace Scene
} // namespace lkRay
