#include "PCH.hpp"
#include "Glass.hpp"

#include "Constants.hpp"
#include "Renderer/SurfaceDistribution.hpp"
#include "Distribution/Transmission.hpp"
#include "Distribution/SpecularReflection.hpp"


namespace lkRay {
namespace Material {

Glass::Glass(const std::string& name)
    : Material(name)
    , mDensity(1.0f)
{
}

Glass::Glass(const std::string& name, const float& density)
    : Material(name)
    , mDensity(density)
{
}

void Glass::SetDensity(const float density)
{
    mDensity = density;
}

void Glass::PopulateDistributionFunctions(Renderer::PathContext& context, Renderer::RayCollision& collision)
{
    collision.mSurfaceDistribution =
        new (*collision.mAllocator) Renderer::SurfaceDistribution(collision.mAllocator);

    // Fresnel equations to determine how much each distribution contributes
    const lkCommon::Math::Vector4& in = collision.mRay.mDirection;
    lkCommon::Math::Vector4 n = collision.mNormal;

    float n1 = 1.0f;
    float n2 = mDensity;

    const float ior = n1 / n2;
    const float sinOmegaInCritical = n2 / n1;
    const float cosOmegaIn = -n.Dot(in);
    const float sin2OmegaIn = 1.0f - (cosOmegaIn * cosOmegaIn);
    const float sinOmegaIn = sqrtf(sin2OmegaIn);
    if (sinOmegaIn > sinOmegaInCritical)
    {
        // total reflection
        collision.mSurfaceDistribution->AddDistribution(
            new (*collision.mAllocator) Distribution::SpecularReflection(1.0f)
        );
        return;
    }

    const float sin2OmegaT = (ior * ior) * sin2OmegaIn;
    const float cosOmegaT = sqrtf(1.0f - sin2OmegaT);

    // Rn - Reflection Normal; Rp - Reflection Parallel
    const float sqrtRn = ((n1 * cosOmegaIn) - (n2 * cosOmegaT)) / ((n1 * cosOmegaIn) + (n2 * cosOmegaT));
    const float Rn = sqrtRn * sqrtRn;
    const float sqrtRp = ((n2 * cosOmegaIn) - (n1 * cosOmegaT)) / ((n2 * cosOmegaIn) + (n1 * cosOmegaT));
    const float Rp = sqrtRp * sqrtRp;

    const float R = 0.5f * (Rn + Rp);
    const float T = 1.0f - R;

    collision.mSurfaceDistribution->AddDistribution(
        new (*collision.mAllocator) Distribution::SpecularReflection(R)
    );

    collision.mSurfaceDistribution->AddDistribution(
        new (*collision.mAllocator) Distribution::Transmission(ior, T)
    );
}

bool Glass::ReadParametersFromNode(const rapidjson::Value& value)
{
    bool densityFound = false;

    for (auto& a : value.GetObject())
    {
        if (Constants::GLASS_ATTRIBUTE_DENSITY_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            densityFound = true;

            if (!a.value.IsFloat())
            {
                LOGE("Invalid density attribute for glass material " << mName <<
                    ". Should be a double.");
                break;
            }

            float d = a.value.GetFloat();

            LOGD("     -> Glass material density " << d);

            SetDensity(d);
            break;
        }
    }

    return densityFound;
}

} // namespace Material
} // namespace lkRay
