#include "PCH.hpp"
#include "SpotLight.hpp"
#include "Constants.hpp"


namespace lkRay {
namespace Light {

SpotLight::SpotLight(const std::string& name)
    : Light(name)
    , mAttentuationFactor(1.0)
{
}

SpotLight::SpotLight(const std::string& name, const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color, float attenuation)
    : Light(name, pos, color)
    , mAttentuationFactor(attenuation)
    , mDirection()
    , mCutoff(0.0f)
{
}

lkCommon::Math::Vector4 SpotLight::GetToLightDir(const Renderer::RayCollision& collision) const
{
    return (mPosition - collision.mPoint);
}

lkCommon::Utils::PixelFloat4 SpotLight::Sample(const Renderer::RayCollision& collision) const
{
    lkCommon::Math::Vector4 lightRayDir(mPosition - collision.mPoint);
    float distToLight = lightRayDir.Length();
    lightRayDir = lightRayDir.Normalize();

    if (mDirection.Dot(lightRayDir) >= mCutoff)
    {
        float lightCoeff = lightRayDir.Dot(collision.mNormal);
        if (lightCoeff < 0.0f)
            lightCoeff = 0.0f;

        return mColor * lightCoeff * (1.0f / (1.0f + mAttentuationFactor * distToLight * distToLight));
    }

    return lkCommon::Utils::PixelFloat4();
}

bool SpotLight::ReadParametersFromNode(const rapidjson::Value& value)
{
    if (!Light::ReadParametersFromNode(value))
    {
        LOGE("Failed to read base light parameters");
        return false;
    }

    bool hasAttenuation = false;
    bool hasDirection = false;
    bool hasCutoff = false;

    // read light's attenuation factor
    for (auto& a: value.GetObject())
    {
        if (Constants::SPOT_LIGHT_ATTRIBUTE_ATTENUATION_FACTOR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            mAttentuationFactor = a.value.GetFloat();
            LOGD("     -> Spot light attenuation factor " << mAttentuationFactor);
            hasAttenuation = true;
            break;
        }
    }

    if (!hasAttenuation)
    {
        LOGE("Error while parsing spot light " << mName << " - missing attFactor parameter");
    }

    // read light's direction
    for (auto& a: value.GetObject())
    {
        if (Constants::SPOT_LIGHT_ATTRIBUTE_DIR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Invalid direction parameter in light " << mName <<
                     ". Should be an array of 3 float numbers.");
                return false;
            }

            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mDirection[colIndex] = c.GetFloat();
                colIndex++;
            }

            mDirection[3] = 0.0f;
            mDirection.Normalize();
            LOGD("     -> Spot light direction " << mDirection);
            hasDirection = true;
            break;
        }
    }

    if (!hasDirection)
    {
        LOGE("Error while parsing spot light " << mName << " - missing direction parameter");
    }

    // read light's attenuation factor
    for (auto& a: value.GetObject())
    {
        if (Constants::SPOT_LIGHT_ATTRIBUTE_CUTOFF_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            mCutoff = a.value.GetFloat();
            LOGD("     -> Spot light cutoff angle " << mCutoff);
            mCutoff = cosf(LKCOMMON_DEG_TO_RADF(mCutoff));
            hasCutoff = true;
            break;
        }
    }

    if (!hasCutoff)
    {
        LOGE("Error while parsing spot light " << mName << " - missing cutoff parameter");
    }

    return hasAttenuation && hasDirection && hasCutoff;
}

} // namespace Light
} // namespace lkRay
