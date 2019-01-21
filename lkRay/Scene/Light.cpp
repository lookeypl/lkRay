#include "PCH.hpp"
#include "Light.hpp"

#include "Constants.hpp"


namespace lkRay {
namespace Scene {

Light::Light(const std::string& name)
    : mName(name)
    , mPosition()
    , mColor(1.0f)
{
}

Light::Light(const std::string& name, const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color)
    : mName(name)
    , mPosition(pos)
    , mColor(color)
{
}

bool Light::ReadPositionFromNode(const rapidjson::Value& value)
{
    for (auto& a: value.GetObject())
    {
        if (Constants::LIGHT_ATTRIBUTE_POSITION_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Provided position node is not an array for light " << mName <<
                     ". Should be an array of 3 floats.");
                return false;
            }

            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mPosition[colIndex] = c.GetFloat();
                colIndex++;
            }

            mPosition[3] = 1.0f;

            LOGD("     -> Light position " << mPosition);
            return true;
        }
    }

    LOGE("Position node not found for light " << mName);
    return false;
}

bool Light::ReadColorFromNode(const rapidjson::Value& value)
{
    for (auto& a: value.GetObject())
    {
        if (Constants::LIGHT_ATTRIBUTE_COLOR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Provided color node is not an array for light " << mName <<
                     ". Should be an array of 3 floats");
                return false;
            }

            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mColor[colIndex] = c.GetFloat();
                colIndex++;
            }

            mColor[3] = 1.0f;
            LOGD("     -> Light color " << mColor);
            return true;
        }
    }

    LOGE("Color node not found for light " << mName);
    return false;
}

bool Light::ReadParametersFromNode(const rapidjson::Value& value)
{
    if (!ReadPositionFromNode(value))
    {
        return false;
    }

    if (!ReadColorFromNode(value))
    {
        return false;
    }

    return true;
}

} // namespace Scene
} // namespace lkRay
