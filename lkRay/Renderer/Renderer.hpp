#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Image.hpp>

#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Geometry/Ray.hpp"


namespace lkRay {
namespace Renderer {

class Renderer final
{
    lkCommon::Utils::Image mOutputImage;
    uint32_t mRenderWidth;
    uint32_t mRenderHeight;

    lkCommon::Math::Vector4 LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor);
    lkCommon::Utils::Image::Pixel CastRay(const Scene::Scene& scene, const Geometry::Ray& ray, int rayCounter);

public:
    Renderer(const uint32_t width, const uint32_t height);
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
