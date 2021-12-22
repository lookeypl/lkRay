#include "PCH.hpp"
#include "Matte.hpp"

#include "Distribution/Lambertian.hpp"
#include "Renderer/SurfaceDistribution.hpp"
#include "Constants.hpp"


namespace lkRay {
namespace Material {

Matte::Matte(const std::string& name)
    : Material(name)
    , mTexture(1, 1, 1, { lkCommon::Utils::PixelFloat4(0.0f) })
{
}

Matte::Matte(const std::string& name, const lkCommon::Utils::PixelFloat4& color)
    : Material(name)
    , mTexture(1, 1, 1, { color })
{
}

bool Matte::LoadTexture(const std::string& path)
{
    return mTexture.Load(path);
}

void Matte::SetColor(const lkCommon::Utils::PixelFloat4& color)
{
    mTexture.Resize(1, 1);
    mTexture.SetPixel(0, 0, color);
}

void Matte::PopulateDistributionFunctions(Renderer::PathContext& context, Renderer::RayCollision& collision)
{
    LKCOMMON_UNUSED(context);

    collision.mSurfaceDistribution =
        new (*collision.mAllocator) Renderer::SurfaceDistribution(collision.mAllocator);

    collision.mSurfaceDistribution->AddDistribution(
        new (*collision.mAllocator) Distribution::Lambertian(
            mTexture.Sample(collision.mUV.u, collision.mUV.v, lkCommon::Utils::Sampling::NEAREST)
        )
    );
}

bool Matte::ReadParametersFromNode(const rapidjson::Value& value)
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
        if (Constants::MATTE_ATTRIBUTE_PATH_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            pathFound = true;

            if (!a.value.IsString())
            {
                LOGE("Invalid path attribute for matte material " << mName <<
                     ". Should be a string.");
                break;
            }

            texLoaded = LoadTexture(a.value.GetString());
            if (!texLoaded)
            {
                LOGE("Failed to read texture from file " << a.value.GetString() <<
                    " for matte material " << mName);
                break;
            }
        }
    }

    if (pathFound)
    {
        return texLoaded;
    }

    for (auto& a: value.GetObject())
    {
        if (Constants::MATTE_ATTRIBUTE_COLOR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Invalid color attribute for matte material " << mName <<
                     ". Should be an array of 3 float numbers.");
                return false;
            }

            uint32_t colIndex = 0;
            lkCommon::Utils::PixelFloat4 color;
            for (auto& c: a.value.GetArray())
            {
                color[colIndex] = c.GetFloat();
                colIndex++;
            }

            LOGD("     -> Matte material color " << color);

            SetColor(color);
            return true;
        }
    }

    LOGE("Color attribute not found for material " << mName);
    return false;
}

} // namespace Material
} // namespace lkRay
