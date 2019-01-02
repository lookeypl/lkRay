#include "PCH.hpp"
#include "Matte.hpp"

#include "Distribution/Lambertian.hpp"


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
    collision.mSurfaceDistribution = new (*collision.mAllocator) Distribution::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(new (*collision.mAllocator) Distribution::Lambertian(mColor));
}

} // namespace Material
} // namespace lkRay
