#include "PCH.hpp"
#include "Mirror.hpp"

#include "Distribution/SpecularReflection.hpp"
#include "Renderer/SurfaceDistribution.hpp"


namespace lkRay {
namespace Material {

Mirror::Mirror(const std::string& name)
    : Material(name)
{
}

void Mirror::PopulateDistributionFunctions(Renderer::PathContext& context, Renderer::RayCollision& collision)
{
    LKCOMMON_UNUSED(context);

    collision.mSurfaceDistribution = new (*collision.mAllocator) Renderer::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(
        new (*collision.mAllocator) Distribution::SpecularReflection(1.0f)
    );
}

bool Mirror::ReadParametersFromNode(const rapidjson::Value& value)
{
    // empty - Mirror has no additional parameters
    LKCOMMON_UNUSED(value);
    return true;
}

} // namespace Material
} // namespace lkRay
