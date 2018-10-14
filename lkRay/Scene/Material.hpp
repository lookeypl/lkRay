#pragma once

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Scene {

class Material final
{
    lkCommon::Utils::PixelFloat4 mColor;
    float mAlbedo;
    float mReflection;
    float mTransparency;

public:
    Material();
    ~Material() = default;

    LKCOMMON_INLINE void SetColor(const lkCommon::Utils::PixelFloat4& color)
    {
        mColor = color;
    }

    LKCOMMON_INLINE void SetAlbedo(float albedo)
    {
        mAlbedo = albedo;
    }

    LKCOMMON_INLINE void SetReflection(float reflection)
    {
        mReflection = reflection;
    }

    LKCOMMON_INLINE void SetTransparency(float transparency)
    {
        mTransparency = transparency;
    }


    LKCOMMON_INLINE const lkCommon::Utils::PixelFloat4& GetColor() const
    {
        return mColor;
    }

    LKCOMMON_INLINE float GetAlbedo() const
    {
        return mAlbedo;
    }

    LKCOMMON_INLINE float GetReflection() const
    {
        return mReflection;
    }

    LKCOMMON_INLINE float GetTransparency() const
    {
        return mTransparency;
    }
};

} // namespace Scene
} // namespace lkRay
