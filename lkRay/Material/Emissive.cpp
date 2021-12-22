#include "PCH.hpp"
#include "Emissive.hpp"

#include "Constants.hpp"
#include "Renderer/SurfaceDistribution.hpp"
#include "Distribution/Emissive.hpp"


namespace lkRay {
namespace Material {


Emissive::Emissive(const std::string& name)
    : Material(name)
    , mIntensity(0.0f)
{
}

Emissive::Emissive(const std::string& name, const lkCommon::Utils::PixelFloat4& intensity)
    : Material(name)
    , mIntensity(intensity)
{
}

void Emissive::SetIntensity(const lkCommon::Utils::PixelFloat4& intensity)
{
    mIntensity = intensity;
}

void Emissive::PopulateDistributionFunctions(Renderer::PathContext& context, Renderer::RayCollision& collision)
{
    LKCOMMON_UNUSED(context);

    collision.mSurfaceDistribution =
        new (*collision.mAllocator) Renderer::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(
        new (*collision.mAllocator) Distribution::Emissive(mIntensity)
    );
}

bool Emissive::ReadParametersFromNode(const rapidjson::Value& value)
{
    if (!value.IsObject())
    {
        LOGE("Invalid node for material " << mName);
        return false;
    }

    bool pathFound = false;
    bool texLoaded = false;

    for (auto& a: value.GetObject())
    {
        if (Constants::EMISSIVE_ATTRIBUTE_INTENSITY_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Invalid intensity attribute for emissive material " << mName <<
                     ". Should be an array of 3 float numbers.");
                return false;
            }

            uint32_t colIndex = 0;
            lkCommon::Utils::PixelFloat4 intensity;
            for (auto& c: a.value.GetArray())
            {
                intensity[colIndex] = c.GetFloat();
                colIndex++;
            }

            intensity[colIndex] = 0.0f;

            LOGD("     -> Emissive material intensity " << intensity);

            SetIntensity(intensity);
            return true;
        }
    }

    LOGE("Intensity attribute not found for material " << mName);
    return false;
}

} // namespace Material
} // namespace lkRay
