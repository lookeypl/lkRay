#include "PCH.hpp"
#include "Glass.hpp"

#include "Constants.hpp"
#include "Renderer/SurfaceDistribution.hpp"
#include "Distribution/Transmission.hpp"


namespace lkRay {
namespace Material {

Glass::Glass(const std::string& name)
    : Material(name)
    , mIOR(1.0f)
{
}

Glass::Glass(const std::string& name, const float& density)
    : Material(name)
    , mIOR(1.0f / density)
{
}

void Glass::SetIOR(const float density)
{
    mIOR = 1.0f / density;
}

void Glass::PopulateDistributionFunctions(Renderer::RayCollision& collision)
{
    collision.mSurfaceDistribution =
        new (*collision.mAllocator) Renderer::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(
        new (*collision.mAllocator) Distribution::Transmission(mIOR)
    );

    if (mIOR > 1.0f)
    {

    }
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

            SetIOR(d);
            break;
        }
    }

    return densityFound;
}

} // namespace Material
} // namespace lkRay
