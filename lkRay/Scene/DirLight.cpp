#include "PCH.hpp"
#include "DirLight.hpp"
#include "Constants.hpp"


namespace lkRay {
namespace Scene {

DirLight::DirLight(const std::string& name)
    : Light(name)
    , mDirection()
{
}

DirLight::DirLight(const std::string& name, const lkCommon::Utils::PixelFloat4& color, const lkCommon::Math::Vector4& dir)
    : Light(name, lkCommon::Math::Vector4(), color)
    , mDirection(dir.Normalize())
{
}

lkCommon::Math::Vector4 DirLight::GetToLightDir(const RayCollision&) const
{
    return mDirection * -10000.0f;
}

lkCommon::Utils::PixelFloat4 DirLight::Sample(const RayCollision& collision) const
{
    float lightCoeff = -mDirection.Dot(collision.mNormal);
    if (lightCoeff < LKCOMMON_EPSILON)
        lightCoeff = 0.0f;

    return mColor * lightCoeff;
}

bool DirLight::ReadParametersFromNode(const rapidjson::Value& value)
{
    if (!Light::ReadColorFromNode(value))
    {
        LOGE("Failed to read dir light color");
        return false;
    }

    // read light's attenuation factor
    for (auto& a: value.GetObject())
    {
        if (Constants::DIR_LIGHT_ATTRIBUTE_DIR_NODE_NAME.compare(a.name.GetString()) == 0)
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
            LOGD("     -> DirLight direction " << mDirection);
            return true;
        }
    }

    LOGE("Dir light direction parameter not found");
    return false;
}

} // namespace Scene
} // namespace lkRay
