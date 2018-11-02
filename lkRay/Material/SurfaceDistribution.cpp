#include "PCH.hpp"
#include "SurfaceDistribution.hpp"

namespace lkRay {
namespace Material {

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
        mDistributions[i]->~Distribution();
        mDistributionAllocator->Free(mDistributions[i]);
    }
}

bool SurfaceDistribution::Sample(DistributionType type, lkCommon::Utils::PixelFloat4& result, lkCommon::Math::Vector4& outDir)
{
    // go through Distributions and find REFLECTION | SPECULAR
    Material::Distribution* distributions[DISTRIBUTION_MAX_COUNT];
    uint32_t distCount = 0;

    for (uint32_t i = 0; i < mDistributionCount; ++i)
    {
        if (mDistributions[i]->HasType(type))
        {
            distributions[distCount] = mDistributions[i];
            distCount++;
        }
    }

    // * sample the distributions and get value of pixel in that place (and out vector)


    return true;
}

} // namespace Material
} // namespace lkRay
