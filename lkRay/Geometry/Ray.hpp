#pragma once

#include <lkCommon/Math/Vector4.hpp>


namespace lkRay {
namespace Geometry {

class Ray
{
    friend class Sphere;

    lkCommon::Math::Vector4 mOrigin;
    lkCommon::Math::Vector4 mDirection;

public:
    Ray(const lkCommon::Math::Vector4& origin, const lkCommon::Math::Vector4& direction);
    ~Ray();

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetOrigin() const
    {
        return mOrigin;
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetDirection() const
    {
        return mDirection;
    }
};

} // namespace Geometry
} // namespace lkRay
