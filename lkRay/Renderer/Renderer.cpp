#include "PCH.hpp"
#include "Renderer.hpp"

#include "Geometry/Primitive.hpp"
#include "Distribution/Function.hpp"
#include "Material/Material.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Math/Utilities.hpp>
#include <lkCommon/Math/Random.hpp>

#include "SurfaceDistribution.hpp"


namespace {

const uint32_t PIXELS_PER_THREAD = 32;
const uint32_t PIXELS_PER_THREAD_CONVERT = 32;

} // namespace


namespace lkRay {
namespace Renderer {

Renderer::Renderer(const uint32_t renderWidth, const uint32_t renderHeight, const uint32_t maxRayDepth, const uint32_t threadCount)
    : mImageBuffer(renderWidth, renderHeight)
    , mOutputImage(renderWidth, renderHeight)
    , mMaxRayDepth(maxRayDepth)
    , mThreadPool(threadCount)
    , mThreadData(mThreadPool.GetWorkerThreadCount())
    , mSampleCount(0)
    , mExposure(1.0f)
{
    for (uint32_t tid = 0; tid < mThreadPool.GetWorkerThreadCount(); ++tid)
    {
        mThreadData[tid].rngState = LKCOMMON_XORSHIFT_INITIAL_STATE;
        mThreadPool.SetUserPayloadForThread(tid, &mThreadData[tid]);
    }
}

lkCommon::Math::Vector4 Renderer::LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor)
{
    return p1 * (1.0f - factor) + p2 * factor;
}

lkCommon::Utils::PixelFloat4 Renderer::PostProcess(const lkCommon::Utils::PixelFloat4& in)
{
    // adjust exposure of raw pixel
    lkCommon::Utils::PixelFloat4 out = in * mExposure;

    // Hejl & Burgess-Dawson gamma correction & tone mapping
    lkCommon::Utils::PixelFloat4 tempPixel = lkCommon::Utils::MaxPixel(lkCommon::Utils::PixelFloat4(0.0f), out - 0.004f);
    out = (tempPixel * (6.2f * tempPixel + 0.5f)) / (tempPixel * (6.2f * tempPixel + 1.7f) + 0.06f);

    return out;
}

lkCommon::Utils::PixelFloat4 Renderer::CalculateLightIntensity(PathContext& context, uint32_t rayDepth)
{
    if (rayDepth > mMaxRayDepth)
    {
        return lkCommon::Utils::PixelFloat4(0.0f);
    }

    if (context.beta[0] < 0.3f &&
        context.beta[1] < 0.3f &&
        context.beta[2] < 0.3f)
    {
        return lkCommon::Utils::PixelFloat4(0.0f);
    }

    const Scene::Scene& scene = context.scene;
    lkCommon::Utils::PixelFloat4 resultColor;
    lkCommon::Utils::PixelFloat4 lightContribution;
    lkCommon::Utils::PixelFloat4 surfaceSample;
    lkCommon::Math::Vector4 reflectedDir;

    RayCollision collision = scene.TestCollision(context.ray, -1);
    if (collision.mHitID == -1)
    {
        return context.beta * scene.GetAmbient();
    }

    collision.mAllocator = &context.threadData.allocator;
    scene.GetPrimitives()[collision.mHitID]->GetMaterial()->PopulateDistributionFunctions(collision);

    bool hasDiffuse = collision.mSurfaceDistribution->Sample(Types::Distribution::DIFFUSE | Types::Distribution::REFLECTION,
                                                             context, collision, surfaceSample, reflectedDir);
    if (hasDiffuse)
    {
        lightContribution = context.beta * scene.SampleLights(collision);

        context.ray.mOrigin = collision.mPoint;
        context.ray.mDirection = reflectedDir;

        // diffuse surfaces weaken our rays - adjust beta
        context.beta *= surfaceSample * (context.ray.mDirection.Dot(collision.mNormal));
        resultColor += lightContribution * surfaceSample + CalculateLightIntensity(context, rayDepth + 1);
        return resultColor;
    }

    bool hasSpecular = collision.mSurfaceDistribution->Sample(Types::Distribution::SPECULAR | Types::Distribution::REFLECTION,
                                                              context, collision, surfaceSample, reflectedDir);
    if (hasSpecular)
    {
        context.ray.mOrigin = collision.mPoint;
        context.ray.mDirection = reflectedDir;

        resultColor += CalculateLightIntensity(context, rayDepth + 1);
        return resultColor;
    }

    return lkCommon::Utils::PixelFloat4(0.0f);
}

void Renderer::DrawThread(lkCommon::Utils::ThreadPayload& payload, const Scene::Scene& scene, const Scene::Camera& camera, uint32_t widthPos, uint32_t heightPos, uint32_t xCount, uint32_t yCount)
{
    LKCOMMON_ASSERT(payload.userData != nullptr, "Thread payload does not contain ArenaAllocator");
    ThreadData* threadData = reinterpret_cast<ThreadData*>(payload.userData);

    for (uint32_t x = widthPos; x < widthPos + xCount; ++x)
    {
        for (uint32_t y = heightPos; y < heightPos + yCount; ++y)
        {
            // don't go out of bounds
            if (x >= mImageBuffer.GetWidth() || y >= mImageBuffer.GetHeight())
                continue;

            // calculate where our ray will shoot
            float xFactor = static_cast<float>(x) / static_cast<float>(mImageBuffer.GetWidth());
            float yFactor = static_cast<float>(y) / static_cast<float>(mImageBuffer.GetHeight());

            // affect factors by random shift to provide pseudo AA
            xFactor += (lkCommon::Math::Random::Xorshift(threadData->rngState) - 0.5f) * mXStep;
            yFactor += (lkCommon::Math::Random::Xorshift(threadData->rngState) - 0.5f) * mYStep;

            lkCommon::Math::Vector4 xLerpTop = LerpPoints(
                camera.GetCameraCorner(Scene::Camera::Corners::TOP_L),
                camera.GetCameraCorner(Scene::Camera::Corners::TOP_R),
                xFactor
            );
            lkCommon::Math::Vector4 xLerpBot = LerpPoints(
                camera.GetCameraCorner(Scene::Camera::Corners::BOT_L),
                camera.GetCameraCorner(Scene::Camera::Corners::BOT_R),
                xFactor
            );
            lkCommon::Math::Vector4 rayTarget(LerpPoints(xLerpTop, xLerpBot, yFactor));
            lkCommon::Math::Vector4 rayDir((rayTarget - camera.GetPosition()).Normalize());

            // form a context and start calculating
            PathContext ctx(*threadData, scene, Geometry::Ray(camera.GetPosition(), rayDir));

            lkCommon::Utils::PixelFloat4 pixel;
            mImageBuffer.GetPixel(x, y, pixel);

            // get the value
            pixel += CalculateLightIntensity(ctx, 0);

            // save current pixel value for later
            mImageBuffer.SetPixel(x, y, pixel);

            // divide by sample count and post-process for output
            pixel /= static_cast<float>(mSampleCount);
            mOutputImage.SetPixel(x, y, PostProcess(pixel));
        }
    }
}

void Renderer::Draw(const Scene::Scene& scene, const Scene::Camera& camera)
{
    mSampleCount++;

    mXStep = 2.0f / static_cast<float>(mImageBuffer.GetWidth());
    mYStep = 2.0f / static_cast<float>(mImageBuffer.GetHeight());

    for (uint32_t x = 0; x < mOutputImage.GetWidth(); x += PIXELS_PER_THREAD)
    {
        for (uint32_t y = 0; y < mOutputImage.GetHeight(); y += PIXELS_PER_THREAD)
        {
            mThreadPool.AddTask(std::bind(&Renderer::DrawThread, this, std::placeholders::_1,
                                scene, camera, x, y, PIXELS_PER_THREAD, PIXELS_PER_THREAD));
        }
    }

    mThreadPool.WaitForTasks();

    for (auto& td: mThreadData)
    {
        td.allocator.ClearUnusedChunks();
    }
}

bool Renderer::ResizeOutput(const uint32_t width, const uint32_t height)
{
    if (width == 0 || height == 0)
    {
        LOGE("Output dimensions cannot be equal to zero");
        return false;
    }

    if (!mImageBuffer.Resize(width, height))
    {
        LOGE("Failed to resize output image");
        return false;
    }

    if (!mOutputImage.Resize(width, height))
    {
        LOGE("Failed to resize output image");
        return false;
    }

    ResetImageBuffer();

    return true;
}

} // namespace Renderer
} // namespace lkRay
