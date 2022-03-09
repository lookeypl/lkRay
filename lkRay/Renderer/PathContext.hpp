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

    PathContext(ThreadData& threadData, const Scene::Scene& scene)
        : threadData(threadData)
        , scene(scene)
    { }
};

} // namespace Renderer
} // namespace lkRay
