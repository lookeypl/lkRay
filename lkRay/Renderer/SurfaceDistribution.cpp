#include "PCH.hpp"
#include "SurfaceDistribution.hpp"


namespace lkRay {
namespace Renderer {

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

bool SurfaceDistribution::Sample(const Types::Distribution type, const PathContext& ctx,
                                 const Scene::RayCollision& collision, lkCommon::Utils::PixelFloat4& surfaceSample,
                                 lkCommon::Math::Vector4& outRayDir)
{
    // go through Distributions and find matching ones
    Distribution::Function* distributions[FUNCTION_MAX_COUNT];
    uint32_t distCount = 0;
    lkCommon::Utils::PixelFloat4 result;

    for (uint32_t i = 0; i < mDistributionCount; ++i)
    {
        if (mDistributions[i]->HasType(type))
        {
            distributions[distCount] = mDistributions[i];
            distCount++;
        }
    }

    if (distCount > 0)
    {
        // sample the distributions and get value of pixel in that place (and out vector)
        for (uint32_t i = 0; i < distCount; ++i)
        {
            surfaceSample += distributions[i]->F(ctx, collision, outRayDir);
        }

        return true;
    }

    return false;
}

} // namespace Renderer
} // namespace lkRay
