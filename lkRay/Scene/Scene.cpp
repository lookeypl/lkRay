#include "PCH.hpp"
#include "Scene.hpp"


namespace lkRay {
namespace Scene {

Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::Load(const std::string& path)
{
    // TODO
    return false;
}

void Scene::AddPrimitive(const Geometry::Primitive::Ptr& ptr)
{
    mPrimitives.push_back(ptr);
}

void Scene::AddLight(const Light::Ptr& ptr)
{
    mLights.push_back(ptr);
}

} // namespace Scene
} // namespace lkRay
