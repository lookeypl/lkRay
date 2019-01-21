#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Pixel.hpp>
#include <memory>

#include "Types.hpp"
#include "RayCollision.hpp"

#include <rapidjson/document.h>


namespace lkRay {
namespace Scene {

class Light
{
protected:
    std::string mName;
    lkCommon::Math::Vector4 mPosition;
    lkCommon::Utils::PixelFloat4 mColor;

    bool ReadPositionFromNode(const rapidjson::Value& value);
    bool ReadColorFromNode(const rapidjson::Value& value);

public:
    Light(const std::string& name);
    Light(const std::string& name, const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color);
    ~Light() = default;

    virtual lkCommon::Math::Vector4 GetToLightDir(const RayCollision& collision) const = 0;
    virtual lkCommon::Utils::PixelFloat4 Sample(const RayCollision& collision) const = 0;
    virtual bool ReadParametersFromNode(const rapidjson::Value& value);

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }

    LKCOMMON_INLINE const lkCommon::Utils::PixelFloat4& GetColor() const
    {
        return mColor;
    }
};

} // namespace Scene
} // namespace lkRay
