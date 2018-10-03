#include "PCH.hpp"
#include "Renderer.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace {

const uint32_t PIXELS_PER_THREAD = 32;
const lkCommon::Utils::PixelFloat4 AMBIENT_COLOR({0.1f, 0.1f, 0.1f, 1.0f});

} // namespace


namespace lkRay {
namespace Renderer {

Renderer::Renderer(const uint32_t renderWidth, const uint32_t renderHeight)
    : mOutputImage(renderWidth, renderHeight)
    , mThreadPool()
{
}

lkCommon::Math::Vector4 Renderer::LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor)
{
    return p1 * (1.0f - factor) + p2 * factor;
}

lkCommon::Utils::PixelFloat4 Renderer::CastRay(const Scene::Scene& scene, const Geometry::Ray& ray, int rayCounter)
{
    lkCommon::Utils::PixelFloat4 resultColor = AMBIENT_COLOR;

    if (rayCounter > 0)
        return resultColor;

    float rayDistance = 0.0f;
    float maxRayDistance = std::numeric_limits<float>::max();
    lkCommon::Math::Vector4 normal;

    for (const auto& p: scene.GetPrimitives())
    {
        if (p->TestCollision(ray, rayDistance, normal))
        {
            if (rayDistance > maxRayDistance)
                continue;

            maxRayDistance = rayDistance;
            float lightCoeff;
            resultColor = AMBIENT_COLOR;
            lkCommon::Math::Vector4 collision = ray.GetOrigin() + ray.GetDirection() * rayDistance;

            for (const auto& l: scene.GetLights())
            {
                lkCommon::Math::Vector4 lightRayDir = l->GetPosition() - collision;
                float colDistance = lightRayDir.Length();
                lightRayDir = lightRayDir.Normalize();

                Geometry::Ray shadowRay(collision, lightRayDir);
                for (const auto& p2: scene.GetPrimitives())
                {
                    // don't test shadow ray collision with ourselves
                    if (p.get() == p2.get())
                        continue;

                    float shadowDist;
                    lkCommon::Math::Vector4 shadowNorm;
                    if (!p2->TestCollision(shadowRay, shadowDist, shadowNorm) || shadowDist >= colDistance)
                    {
                        lightCoeff = lightRayDir.Dot(normal);
                        if (lightCoeff < 0.0f)
                            lightCoeff = 0.0f;

                        float att = (1.0f / (1.0f + l->GetAttenuationFactor() * colDistance * colDistance));

                        resultColor += l->GetColor() * lightCoeff * att;
                    }
                }
            }
        }
    }

    return resultColor;
}

void Renderer::DrawThread(const Scene::Scene& scene, const Scene::Camera& camera, uint32_t widthPos, uint32_t heightPos, uint32_t xCount, uint32_t yCount)
{
    for (uint32_t x = widthPos; x < widthPos + xCount; ++x)
    {
        for (uint32_t y = heightPos; y < heightPos + yCount; ++y)
        {
            // don't go out of bounds
            if (x >= mOutputImage.GetWidth() || y >= mOutputImage.GetHeight())
                continue;

            // calculate where our ray will shoot
            float xFactor = static_cast<float>(x) / static_cast<float>(mOutputImage.GetWidth());
            float yFactor = static_cast<float>(y) / static_cast<float>(mOutputImage.GetHeight());

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
            lkCommon::Math::Vector4 rayTarget(LerpPoints(xLerp1, xLerp2, yFactor));
            lkCommon::Math::Vector4 rayDir((rayTarget - camera.GetPosition()).Normalize());

            // form a ray and cast it
            Geometry::Ray ray(camera.GetPosition(), rayDir);
            mOutputImage.SetPixel(x, y, CastRay(scene, ray, 0));
        }
    }
}

void Renderer::Draw(const Scene::Scene& scene, const Scene::Camera& camera)
{
    for (uint32_t x = 0; x < mOutputImage.GetWidth(); x += PIXELS_PER_THREAD)
    {
        for (uint32_t y = 0; y < mOutputImage.GetHeight(); y += PIXELS_PER_THREAD)
        {
            mThreadPool.AddTask(std::bind(&Renderer::DrawThread, this, scene, camera, x, y, PIXELS_PER_THREAD, PIXELS_PER_THREAD));
        }
    }

    mThreadPool.WaitForTasks();
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

    return true;
}

} // namespace Renderer
} // namespace lkRay
