#include "PCH.hpp"
#include "Renderer.hpp"

#include "Geometry/Primitive.hpp"
#include "Material/Distribution.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace {

const uint32_t PIXELS_PER_THREAD = 32;

} // namespace


namespace lkRay {
namespace Renderer {

Renderer::Renderer(const uint32_t renderWidth, const uint32_t renderHeight, const uint32_t maxRayDepth)
    : mOutputImage(renderWidth, renderHeight)
    , mMaxRayDepth(maxRayDepth)
    , mThreadPool()
    , mArenas(mThreadPool.GetWorkerThreadCount())
{
    for (uint32_t tid = 0; tid < mThreadPool.GetWorkerThreadCount(); ++tid)
    {
        mThreadPool.SetUserPayloadForThread(tid, &mArenas[tid]);
    }
}

lkCommon::Math::Vector4 Renderer::LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor)
{
    return p1 * (1.0f - factor) + p2 * factor;
}

lkCommon::Utils::PixelFloat4 Renderer::GetSpecularReflection(const Scene::Scene& scene, const Scene::RayCollision& collision,
                                                             uint32_t rayDepth)
{
    lkCommon::Utils::PixelFloat4 surfaceSample;
    lkCommon::Math::Vector4 reflectedDir;

    // sample all specular reflection-related distributions from given surface
    bool hasSpecular = collision.mSurfaceDistribution->Sample(
        Material::DistributionType::SPECULAR | Material::DistributionType::REFLECTION,
        surfaceSample,
        reflectedDir
    );

    if (hasSpecular)
    {
        // recast ray in direction
    }

    return surfaceSample;
}

lkCommon::Utils::PixelFloat4 Renderer::CalculateLightIntensity(lkCommon::Utils::ArenaAllocator* allocator, const Scene::Scene& scene, const Geometry::Ray& ray, uint32_t rayDepth)
{
    lkCommon::Utils::PixelFloat4 resultColor;

    Scene::RayCollision collision = scene.TestCollision(ray, -1);
    if (collision.mHitID == -1)
    {
        return scene.GetAmbient();
    }

    collision.mAllocator = allocator;
    scene.GetPrimitives()[collision.mHitID]->GetMaterial()->PopulateDistributionFunctions(collision);

    resultColor += scene.SampleLights(collision);

    if (rayDepth + 1 < mMaxRayDepth)
    {
        resultColor += GetSpecularReflection(scene, collision, rayDepth);
    }

    return resultColor;
}

void Renderer::DrawThread(lkCommon::Utils::ThreadPayload& payload, const Scene::Scene& scene, const Scene::Camera& camera, uint32_t widthPos, uint32_t heightPos, uint32_t xCount, uint32_t yCount)
{
    LKCOMMON_ASSERT(payload.userData != nullptr, "Thread payload does not contain ArenaAllocator");
    lkCommon::Utils::ArenaAllocator* allocator =
        reinterpret_cast<lkCommon::Utils::ArenaAllocator*>(payload.userData);

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
            mOutputImage.SetPixel(x, y, CalculateLightIntensity(allocator, scene, ray));
        }
    }
}

void Renderer::Draw(const Scene::Scene& scene, const Scene::Camera& camera)
{
    for (uint32_t x = 0; x < mOutputImage.GetWidth(); x += PIXELS_PER_THREAD)
    {
        for (uint32_t y = 0; y < mOutputImage.GetHeight(); y += PIXELS_PER_THREAD)
        {
            mThreadPool.AddTask(std::bind(&Renderer::DrawThread, this, std::placeholders::_1,
                                scene, camera, x, y, PIXELS_PER_THREAD, PIXELS_PER_THREAD));
        }
    }

    mThreadPool.WaitForTasks();

    for (auto& arena: mArenas)
    {
        arena.ClearUnusedChunks();
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

    return true;
}

} // namespace Renderer
} // namespace lkRay
