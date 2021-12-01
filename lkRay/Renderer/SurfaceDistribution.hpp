#pragma once

#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Allocators/MemoryObject.hpp>
#include <lkCommon/Allocators/ArenaAllocator.hpp>

#include "Distribution/Function.hpp"
#include "Types.hpp"

#include "PathContext.hpp"
#include "RayCollision.hpp"

#include <vector>
#include <memory>

#define FUNCTION_MAX_COUNT 8

namespace lkRay {
namespace Renderer {

class SurfaceDistribution: public lkCommon::Allocators::MemoryObject<lkCommon::Allocators::ArenaAllocator>
{
    lkCommon::Allocators::Memory<lkCommon::Allocators::ArenaAllocator>* mDistributionAllocator;
    Distribution::Function* mDistributions[FUNCTION_MAX_COUNT];
    uint32_t mDistributionCount;

public:
    SurfaceDistribution(lkCommon::Allocators::Memory<lkCommon::Allocators::ArenaAllocator>* allocator);
    ~SurfaceDistribution();

    // Returns contribution of surface distribution functions for given collision point, as well as direction of reflected ray.
    // If no distributions match type @p type, returns 0.0f and outDir is intact.
    bool Sample(const Types::Distribution type, const PathContext& ctx,
                const RayCollision& collision, lkCommon::Utils::PixelFloat4& surfaceSample,
                lkCommon::Math::Vector4& outRayDir);

    LKCOMMON_INLINE void AddDistribution(Distribution::Function* dist)
    {
        LKCOMMON_ASSERT(mDistributionCount < FUNCTION_MAX_COUNT, "Too many distribution functions added to given surface distribution");

        mDistributions[mDistributionCount] = dist;
        ++mDistributionCount;
    }

    LKCOMMON_INLINE Distribution::Function* GetDistribution(uint32_t i) const
    {
        LKCOMMON_ASSERT(i < mDistributionCount, "Requested distribution exceeding range");
        LKCOMMON_ASSERT(mDistributions[i] != nullptr, "Requested NULL distribution");

        return mDistributions[i];
    }

    LKCOMMON_INLINE uint32_t GetDistributionCount() const
    {
        return mDistributionCount;
    }
};

} // namespace Renderer
} // namespace lkRay
