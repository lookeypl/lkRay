#pragma once

#include <lkCommon/Math/Vector4.hpp>


namespace lkRay {
namespace Geometry {

struct Ray
{
    lkCommon::Math::Vector4 mOrigin;
    lkCommon::Math::Vector4 mDirection;

    Ray(const lkCommon::Math::Vector4& origin, const lkCommon::Math::Vector4& direction)
        : mOrigin(origin)
        , mDirection(direction)
    {
    }

    ~Ray() = default;
};

} // namespace Geometry
} // namespace lkRay
