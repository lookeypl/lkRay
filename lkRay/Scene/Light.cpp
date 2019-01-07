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

bool Light::ReadParametersFromNode(const rapidjson::Value& value)
{
    for (auto& a: value.GetObject())
    {
        if (Constants::LIGHT_ATTRIBUTE_POSITION_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mPosition[colIndex] = c.GetFloat();
                colIndex++;
            }

            LOGD("     -> Light position " << mPosition);
        }
        else if (Constants::LIGHT_ATTRIBUTE_COLOR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mColor[colIndex] = c.GetFloat();
                colIndex++;
            }

            LOGD("     -> Light color " << mColor);
        }
    }

    return true;
}

} // namespace Scene
} // namespace lkRay
