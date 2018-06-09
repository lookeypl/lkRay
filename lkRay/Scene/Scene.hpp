#pragma once

#include <string>
#include <vector>
#include "Geometry/Primitive.hpp"
#include "Scene/Light.hpp"


namespace lkRay {
namespace Scene {

class Scene final
{
    template <typename T>
    using Container = std::vector<T>;

    Container<Geometry::Primitive::Ptr> mPrimitives; // TODO create Scene::Object
    Container<Light::Ptr> mLights;

public:
    Scene();
    ~Scene();

    bool Load(const std::string& path);

    void AddPrimitive(Geometry::Primitive::Ptr& ptr);
    void AddLight(Light::Ptr& ptr);

    LKCOMMON_INLINE const Container<Geometry::Primitive::Ptr>& GetPrimitives() const
    {
        return mPrimitives;
    }

    LKCOMMON_INLINE const Container<Light::Ptr>& GetLights() const
    {
        return mLights;
    }
};

} // namespace Scene
} // namespace lkRay
