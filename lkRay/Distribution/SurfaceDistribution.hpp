#pragma once

#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>
#include <lkCommon/Utils/ArenaObject.hpp>

#include "Function.hpp"

#include <vector>
#include <memory>

#define FUNCTION_MAX_COUNT 8

namespace lkRay {
namespace Distribution {

class SurfaceDistribution: public lkCommon::Utils::ArenaObject
{
    lkCommon::Utils::ArenaAllocator* mDistributionAllocator;
    Function* mDistributions[FUNCTION_MAX_COUNT];
    uint32_t mDistributionCount;

public:
    SurfaceDistribution(lkCommon::Utils::ArenaAllocator* allocator);
    ~SurfaceDistribution();

    // true if result of sampling was successful - surface had distributions of requested type
    bool Sample(FunctionType type, const lkCommon::Math::Vector4& in, lkCommon::Math::Vector4& normal,
                lkCommon::Utils::PixelFloat4& result, lkCommon::Math::Vector4& outDir);

    LKCOMMON_INLINE void AddDistribution(Function* dist)
    {
        LKCOMMON_ASSERT(mDistributionCount < FUNCTION_MAX_COUNT, "Too many distribution functions added to given surface distribution");

        mDistributions[mDistributionCount] = dist;
        ++mDistributionCount;
    }

    LKCOMMON_INLINE Function* GetDistribution(uint32_t i) const
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

} // namespace Distribution
} // namespace lkRay
