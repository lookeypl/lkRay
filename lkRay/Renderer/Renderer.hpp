#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Image.hpp>
#include <lkCommon/Utils/ThreadPool.hpp>
#include <lkCommon/Allocators/ArenaAllocator.hpp>

#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Geometry/Ray.hpp"

#include "ThreadData.hpp"
#include "PathContext.hpp"


namespace lkRay {
namespace Renderer {

class Renderer final
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelFloat4> mImageBuffer;
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> mOutputImage;
    float mExposure;
    float mXStep;
    float mYStep;
    uint32_t mMaxRayDepth;
    lkCommon::Utils::ThreadPool mThreadPool;
    std::vector<ThreadData> mThreadData;
    uint32_t mSampleCount;

    lkCommon::Math::Vector4 LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor);
    lkCommon::Utils::PixelFloat4 PostProcess(const lkCommon::Utils::PixelFloat4& in);
    void DrawThread(lkCommon::Utils::ThreadPayload& payload,
                    const Scene::Scene& scene, const Scene::Camera& camera,
                    uint32_t widthPos, uint32_t heightPos,
                    uint32_t xCount, uint32_t yCount);
    void ConvertImageBufferToOutputThread(lkCommon::Utils::ThreadPayload& payload, uint32_t widthPos, uint32_t heightPos, uint32_t xCount, uint32_t yCount);

    lkCommon::Utils::PixelFloat4 CalculateLightIntensity(PathContext& context, uint32_t rayDepth);

public:
    Renderer(const uint32_t width, const uint32_t height, const uint32_t maxRayDepth, const uint32_t threadCount);
    ~Renderer() = default;

    void Draw(const Scene::Scene& scene, const Scene::Camera& camera);
    bool ResizeOutput(const uint32_t width, const uint32_t height);

    LKCOMMON_INLINE void ResetImageBuffer()
    {
        mSampleCount = 0;
        mImageBuffer.SetAllPixels(lkCommon::Utils::PixelFloat4(0.0f));
        mOutputImage.SetAllPixels(lkCommon::Utils::PixelFloat4(0.0f));
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

    LKCOMMON_INLINE uint32_t GetRayDepth() const
    {
        return mMaxRayDepth;
    }
};

} // namespace Renderer
} // namespace lkRay
