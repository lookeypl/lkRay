#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Pixel.hpp>
#include <memory>

#include "RayCollision.hpp"


namespace lkRay {
namespace Scene {

enum class LightType
{
    UNKNOWN = 0,
    POINT,
};

class Light
{
public:
    using Ptr = std::shared_ptr<Light>;

protected:
    lkCommon::Math::Vector4 mPosition;
    lkCommon::Utils::PixelFloat4 mColor;

public:
    Light(const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color)
        : mPosition(pos)
        , mColor(color)
    {
    }

    ~Light() = default;

    virtual lkCommon::Utils::PixelFloat4 Sample(const RayCollision& collision) const = 0;
    virtual LightType GetType() const = 0;

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
