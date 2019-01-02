#include "PCH.hpp"
#include "SurfaceDistribution.hpp"

namespace lkRay {
namespace Distribution {

SurfaceDistribution::SurfaceDistribution(lkCommon::Utils::ArenaAllocator* allocator)
    : mDistributionAllocator(allocator)
    , mDistributions{nullptr,}
    , mDistributionCount(0)
{
}

SurfaceDistribution::~SurfaceDistribution()
{
    for (uint32_t i = 0; i < mDistributionCount; ++i)
    {
        mDistributions[i]->~Function();
        mDistributionAllocator->Free(mDistributions[i]);
    }
}

bool SurfaceDistribution::Sample(FunctionType type, const lkCommon::Math::Vector4& in, lkCommon::Math::Vector4& normal, lkCommon::Utils::PixelFloat4& result, lkCommon::Math::Vector4& outDir)
{
    // go through Distributions and find matching
    Function* distributions[FUNCTION_MAX_COUNT];
    uint32_t distCount = 0;

    for (uint32_t i = 0; i < mDistributionCount; ++i)
    {
        if (mDistributions[i]->HasType(type))
        {
            distributions[distCount] = mDistributions[i];
            distCount++;
        }
    }

    if (distCount == 0)
        return false;

    // sample the distributions and get value of pixel in that place (and out vector)
    result = 0.0f;
    for (uint32_t i = 0; i < distCount; ++i)
    {
        result += distributions[i]->F(in, normal, outDir);
    }

    return true;
}


} // namespace Distribution
} // namespace lkRay