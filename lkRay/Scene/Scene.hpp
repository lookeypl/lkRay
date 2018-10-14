#pragma once

#include <string>
#include <vector>
#include "Geometry/Primitive.hpp"
#include "Scene/Light.hpp"


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

public:
    Scene();
    ~Scene();

    bool Load(const std::string& path);

    void AddPrimitive(Geometry::Primitive::Ptr& ptr);
    void AddLight(Light::Ptr& ptr);

    LKCOMMON_INLINE const PrimitiveContainer& GetPrimitives() const
    {
        return mPrimitives;
    }

    LKCOMMON_INLINE const LightContainer& GetLights() const
    {
        return mLights;
    }
};

} // namespace Scene
} // namespace lkRay
