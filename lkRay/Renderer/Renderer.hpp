#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Image.hpp>
#include <lkCommon/Utils/ThreadPool.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>

#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Geometry/Ray.hpp"


namespace lkRay {
namespace Renderer {

class Renderer final
{
    lkCommon::Utils::Image mOutputImage;
    uint32_t mMaxRayDepth;
    lkCommon::Utils::ThreadPool mThreadPool;
    std::vector<lkCommon::Utils::ArenaAllocator> mArenas;

    lkCommon::Math::Vector4 LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor);
    void DrawThread(lkCommon::Utils::ThreadPayload& payload,
                    const Scene::Scene& scene, const Scene::Camera& camera,
                    uint32_t widthPos, uint32_t heightPos,
                    uint32_t xCount, uint32_t yCount);

    lkCommon::Utils::PixelFloat4 GetSpecularReflection(const Scene::Scene& scene, const Scene::RayCollision& collision,
                                                       uint32_t rayDepth);
    lkCommon::Utils::PixelFloat4 CalculateLightIntensity(lkCommon::Utils::ArenaAllocator* allocator, const Scene::Scene& scene, const Geometry::Ray& ray, uint32_t rayDepth = 0);

public:
    Renderer(const uint32_t width, const uint32_t height, const uint32_t maxRayDepth);
    ~Renderer() = default;

    void Draw(const Scene::Scene& scene, const Scene::Camera& camera);
    bool ResizeOutput(const uint32_t width, const uint32_t height);

    LKCOMMON_INLINE const lkCommon::Utils::Image& GetOutput() const
    {
        return mOutputImage;
    }
};

} // namespace Renderer
} // namespace lkRay
