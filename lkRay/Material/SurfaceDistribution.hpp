#pragma once

#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>
#include <lkCommon/Utils/ArenaObject.hpp>

#include "Distribution.hpp"

#include <vector>
#include <memory>

#define DISTRIBUTION_MAX_COUNT 8

namespace lkRay {
namespace Material {

class SurfaceDistribution: public lkCommon::Utils::ArenaObject
{
    lkCommon::Utils::ArenaAllocator* mDistributionAllocator;
    Distribution* mDistributions[DISTRIBUTION_MAX_COUNT];
    uint32_t mDistributionCount;

public:
    SurfaceDistribution(lkCommon::Utils::ArenaAllocator* allocator);
    ~SurfaceDistribution();

    // true if result of sampling was successful, ex. surface had distributions of requested type
    bool Sample(DistributionType type, lkCommon::Utils::PixelFloat4& result, lkCommon::Math::Vector4& outDir);

    LKCOMMON_INLINE void AddDistribution(Distribution* dist)
    {
        LKCOMMON_ASSERT(mDistributionCount < DISTRIBUTION_MAX_COUNT, "Too many distributions added to given surface distribution");

        mDistributions[mDistributionCount] = dist;
        ++mDistributionCount;
    }

    LKCOMMON_INLINE Distribution* GetDistribution(uint32_t i) const
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

} // namespace Material
} // namespace lkRay
