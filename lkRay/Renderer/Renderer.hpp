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
    struct ThreadData
    {
        lkCommon::Utils::ArenaAllocator allocator;
        uint32_t rngState;
    };

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

    lkCommon::Utils::Image<lkCommon::Utils::PixelFloat4> mImageBuffer;
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> mOutputImage;
    float mExposure;
    uint32_t mMaxRayDepth;
    lkCommon::Utils::ThreadPool mThreadPool;
    std::vector<ThreadData> mThreadData;
    uint32_t mSampleCount;

    lkCommon::Math::Vector4 LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor);
    void DrawThread(lkCommon::Utils::ThreadPayload& payload,
                    const Scene::Scene& scene, const Scene::Camera& camera,
                    uint32_t widthPos, uint32_t heightPos,
                    uint32_t xCount, uint32_t yCount);
    void ConvertImageBufferToOutputThread(lkCommon::Utils::ThreadPayload& payload, uint32_t widthPos, uint32_t heightPos, uint32_t xCount, uint32_t yCount);

    lkCommon::Utils::PixelFloat4 GetDiffuseReflection(Renderer::PathContext& context, Scene::RayCollision& collision, uint32_t rayDepth);
    lkCommon::Utils::PixelFloat4 GetSpecularReflection(Renderer::PathContext& context, Scene::RayCollision& collision, uint32_t rayDepth);
    lkCommon::Utils::PixelFloat4 CalculateLightIntensity(Renderer::PathContext& context, uint32_t rayDepth);

public:
    Renderer(const uint32_t width, const uint32_t height, const uint32_t maxRayDepth);
    ~Renderer() = default;

    void Draw(const Scene::Scene& scene, const Scene::Camera& camera);
    bool ResizeOutput(const uint32_t width, const uint32_t height);

    LKCOMMON_INLINE void ResetImageBuffer()
    {
        mSampleCount = 0;
        mImageBuffer.SetAllPixels(lkCommon::Utils::PixelFloat4(0.0f));
    }

    LKCOMMON_INLINE void SetMaxRayDepth(uint32_t depth)
    {
        mMaxRayDepth = depth;
    }

    LKCOMMON_INLINE void SetExposure(float exposure)
    {
        mExposure = exposure;
    }

    LKCOMMON_INLINE const lkCommon::Utils::Image<lkCommon::Utils::PixelUint4>& GetOutput() const
    {
        return mOutputImage;
    }

    LKCOMMON_INLINE uint32_t GetSampleCount() const
    {
        return mSampleCount;
    }
};

} // namespace Renderer
} // namespace lkRay
