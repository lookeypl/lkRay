#include "PCH.hpp"
#include "Renderer.hpp"

#include <lkCommon/Utils/Logger.hpp>


namespace lkRay {
namespace Renderer {

Renderer::Renderer(const uint32_t renderWidth, const uint32_t renderHeight)
    : mOutputImage()
    , mRenderWidth(renderWidth)
    , mRenderHeight(renderHeight)
{
    mOutputImage.Resize(renderWidth, renderHeight);
}

lkCommon::Math::Vector4 Renderer::LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor)
{
    return p1 * (1.0f - factor) + p2 * factor;
}

lkCommon::Utils::Pixel<float, 4> Renderer::CastRay(const Scene::Scene& scene, const Geometry::Ray& ray, int rayCounter)
{
    lkCommon::Utils::Pixel<float, 4> resultColor;

    if (rayCounter > 0)
        return resultColor;

    float rayDistance = 0.0f;
    lkCommon::Math::Vector4 normal;

    for (const auto& p: scene.GetPrimitives())
    {
        if (p->TestCollision(ray, rayDistance, normal))
        {
            float lightCoeff;

            for (const auto& l: scene.GetLights())
            {
                lkCommon::Math::Vector4 collision = ray.GetOrigin() + ray.GetDirection() * rayDistance;
                lkCommon::Math::Vector4 lightRayDir = l->GetPosition() - collision;
                lightRayDir.Normalize();

                lightCoeff = lightRayDir.Dot(normal);
                if (lightCoeff < 0.0f)
                    lightCoeff = 0.0f;
            }

            resultColor.mColors[0] = lightCoeff;
        }
        else
        {
            resultColor.mColors[0] = 0.1f;
            resultColor.mColors[1] = 0.1f;
            resultColor.mColors[2] = 0.1f;
        }
    }

    return resultColor;
}

void Renderer::Draw(const Scene::Scene& scene, const Scene::Camera& camera)
{
    for (uint32_t x = 0; x < mRenderWidth; ++x)
    {
        for (uint32_t y = 0; y < mRenderHeight; ++y)
        {
            // calculate where our ray will shoot
            float xFactor = static_cast<float>(x) / static_cast<float>(mRenderWidth);
            float yFactor = static_cast<float>(y) / static_cast<float>(mRenderHeight);

            lkCommon::Math::Vector4 xLerp1 = LerpPoints(
                camera.GetCameraCorner(Scene::Camera::Corners::TOP_L),
                camera.GetCameraCorner(Scene::Camera::Corners::TOP_R),
                xFactor
            );
            lkCommon::Math::Vector4 xLerp2 = LerpPoints(
                camera.GetCameraCorner(Scene::Camera::Corners::BOT_L),
                camera.GetCameraCorner(Scene::Camera::Corners::BOT_R),
                xFactor
            );
            lkCommon::Math::Vector4 rayTarget = LerpPoints(xLerp1, xLerp2, yFactor);
            lkCommon::Math::Vector4 rayDir = rayTarget - camera.GetPosition();
            rayDir.Normalize();

            // form a ray and cast it
            Geometry::Ray ray(camera.GetPosition(), rayDir);
            mOutputImage.SetPixel(x, y, CastRay(scene, ray, 0));
        }
    }
}

bool Renderer::ResizeOutput(const uint32_t width, const uint32_t height)
{
    if (width == 0 || height == 0)
    {
        LOGE("Output dimensions cannot be equal to zero");
        return false;
    }

    if (!mOutputImage.Resize(width, height))
    {
        LOGE("Failed to resize output image");
        return false;
    }

    mRenderWidth = width;
    mRenderHeight = height;
    return true;
}

} // namespace Renderer
} // namespace lkRay
