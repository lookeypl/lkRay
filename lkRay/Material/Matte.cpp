#include "PCH.hpp"
#include "Matte.hpp"

#include "Distribution/Lambertian.hpp"


namespace {

const std::string MATTE_ATTRIBUTE_COLOR_NODE_NAME = "color";

} // namespace


namespace lkRay {
namespace Material {

Matte::Matte()
    : Material(Types::Material::MATTE)
    , mColor()
{
}

Matte::Matte(const lkCommon::Utils::PixelFloat4& color)
    : Material(Types::Material::MATTE)
    , mColor(color)
{
}

void Matte::PopulateDistributionFunctions(Scene::RayCollision& collision)
{
    collision.mSurfaceDistribution = new (*collision.mAllocator) Distribution::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(new (*collision.mAllocator) Distribution::Lambertian(mColor));
}

bool Matte::ReadParametersFromNode(const rapidjson::Value& value)
{
    for (auto& a: value.GetObject())
    {
        if (MATTE_ATTRIBUTE_COLOR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mColor.mColors.f[colIndex] = c.GetFloat();
                colIndex++;
            }

            LOGD("     -> Matte material color " << mColor);
            break;
        }
    }

    return true;
}

} // namespace Material
} // namespace lkRay
