#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Pixel.hpp>
#include <memory>

namespace lkRay {
namespace Scene {

class Light
{
public:
    using Ptr = std::shared_ptr<Light>;

private:
    lkCommon::Math::Vector4 mPosition;
    lkCommon::Utils::PixelFloat4 mColor;
    float mAttentuationFactor;

public:
    Light(const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color, float attenuation);
    ~Light() = default;

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }

    LKCOMMON_INLINE const lkCommon::Utils::PixelFloat4& GetColor() const
    {
        return mColor;
    }

    LKCOMMON_INLINE const float& GetAttenuationFactor() const
    {
        return mAttentuationFactor;
    }
};

} // namespace Scene
} // namespace lkRay
