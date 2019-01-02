#include "PCH.hpp"
#include "Mirror.hpp"

#include "Distribution/SpecularReflection.hpp"


namespace lkRay {
namespace Material {

Mirror::Mirror()
    : Material(MaterialType::MIRROR)
{
}

void Mirror::PopulateDistributionFunctions(Scene::RayCollision& collision)
{
    collision.mSurfaceDistribution = new (*collision.mAllocator) Distribution::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(new (*collision.mAllocator) Distribution::SpecularReflection());
}

} // namespace Material
} // namespace lkRay
