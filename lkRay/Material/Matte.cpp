#include "PCH.hpp"
#include "Matte.hpp"

#include "LambertianDistribution.hpp"


namespace lkRay {
namespace Material {

Matte::Matte()
    : Material(MaterialType::MATTE)
    , mColor()
{
}

Matte::Matte(const lkCommon::Utils::PixelFloat4& color)
    : Material(MaterialType::MATTE)
    , mColor(color)
{
}

void Matte::PopulateDistributionFunctions(Scene::RayCollision& collision)
{
    collision.mSurfaceDistribution = new (*collision.mAllocator) SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(new (*collision.mAllocator) LambertianDistribution(mColor));
}

} // namespace Material
} // namespace lkRay
