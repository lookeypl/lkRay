#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Image.hpp>
#include <lkCommon/Utils/ThreadPool.hpp>

#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Geometry/Ray.hpp"


namespace lkRay {
namespace Renderer {

class Renderer final
{
    lkCommon::Utils::Image mOutputImage;
    lkCommon::Utils::ThreadPool mThreadPool;

    lkCommon::Math::Vector4 LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor);
    int32_t TestCollision(const Scene::Scene::Container<Geometry::Primitive>& prims, const Geometry::Ray& ray,
                          int skipObjID, float& distance, lkCommon::Math::Vector4& normal);
    lkCommon::Utils::PixelFloat4 CastRay(const Scene::Scene& scene, const Geometry::Ray& primaryRay, int rayCounter);

public:
    Renderer(const uint32_t width, const uint32_t height);
    ~Renderer() = default;

    void DrawThread(const Scene::Scene& scene, const Scene::Camera& camera, uint32_t widthPos, uint32_t heightPos, uint32_t xCount, uint32_t yCount);
    void Draw(const Scene::Scene& scene, const Scene::Camera& camera);
    bool ResizeOutput(const uint32_t width, const uint32_t height);

    LKCOMMON_INLINE const lkCommon::Utils::Image& GetOutput() const
    {
        return mOutputImage;
    }
};

} // namespace Renderer
} // namespace lkRay
