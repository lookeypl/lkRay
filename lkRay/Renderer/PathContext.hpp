#pragma once

#include <lkCommon/Utils/Pixel.hpp>

#include "ThreadData.hpp"
#include "Scene/Scene.hpp"
#include "Geometry/Ray.hpp"


namespace lkRay {
namespace Renderer {

struct PathContext
{
    ThreadData& threadData;
    const Scene::Scene& scene;
    Geometry::Ray ray;

    PathContext(ThreadData& threadData, const Scene::Scene& scene, const Geometry::Ray& ray)
        : threadData(threadData)
        , scene(scene)
        , ray(ray)
    { }
};

} // namespace Renderer
} // namespace lkRay
