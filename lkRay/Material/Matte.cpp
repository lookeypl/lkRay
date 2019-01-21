#include "PCH.hpp"
#include "Matte.hpp"

#include "Distribution/Lambertian.hpp"
#include "Renderer/SurfaceDistribution.hpp"


namespace {

const std::string MATTE_ATTRIBUTE_COLOR_NODE_NAME = "color";

} // namespace


namespace lkRay {
namespace Material {

Matte::Matte(const std::string& name)
    : Material(name)
    , mColor()
{
}

Matte::Matte(const std::string& name, const lkCommon::Utils::PixelFloat4& color)
    : Material(name)
    , mColor(color)
{
}

void Matte::PopulateDistributionFunctions(Scene::RayCollision& collision)
{
    collision.mSurfaceDistribution = new (*collision.mAllocator) Renderer::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(new (*collision.mAllocator) Distribution::Lambertian(mColor));
}

bool Matte::ReadParametersFromNode(const rapidjson::Value& value)
{
    if (!value.IsObject())
    {
        LOGE("Invalid node for material " << mName);
        return false;
    }

    for (auto& a: value.GetObject())
    {
        if (MATTE_ATTRIBUTE_COLOR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Invalid color attribute for matte material " << mName <<
                     ". Should be an array of 3 float numbers.");
                return false;
            }

            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mColor.mColors.f[colIndex] = c.GetFloat();
                colIndex++;
            }

            LOGD("     -> Matte material color " << mColor);
            return true;
        }
    }

    LOGE("Color attribute not found for material " << mName);
    return false;
}

} // namespace Material
} // namespace lkRay
