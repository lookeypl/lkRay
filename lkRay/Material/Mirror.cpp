#include "PCH.hpp"
#include "Mirror.hpp"

#include "LambertianDistribution.hpp"
#include "SpecularReflection.hpp"


namespace lkRay {
namespace Material {

Mirror::Mirror()
    : Material(MaterialType::MIRROR)
{
}

void Mirror::PopulateDistributionFunctions(Scene::RayCollision& collision)
{
    collision.mSurfaceDistribution = new (*collision.mAllocator) SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(new (*collision.mAllocator) SpecularReflection());
}

} // namespace Material
} // namespace lkRay
