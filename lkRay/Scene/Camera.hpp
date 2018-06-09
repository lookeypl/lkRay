#pragma once

#include "lkCommon/Math/Vector4.hpp"


namespace lkRay {
namespace Scene {

class Camera
{
public:
    enum Corners
    {
        TOP_L = 0,
        TOP_R,
        BOT_L,
        BOT_R,
    };

private:
    lkCommon::Math::Vector4 mPosition;
    lkCommon::Math::Vector4 mDirection;
    lkCommon::Math::Vector4 mUp;
    lkCommon::Math::Vector4 mCameraCorners[4];
    float mHalfFovRad;
    float mAspectRatio;
    bool mNeedsUpdate;

public:
    Camera(const lkCommon::Math::Vector4& pos, const lkCommon::Math::Vector4& dir, const lkCommon::Math::Vector4& up, const float horizontalFov, const float aspect);
    ~Camera() = default;

    void UpdateCorners();

    LKCOMMON_INLINE void SetPosition(const lkCommon::Math::Vector4& pos)
    {
        mPosition = pos;
        mNeedsUpdate = true;
    }

    LKCOMMON_INLINE void SetDirection(const lkCommon::Math::Vector4& dir)
    {
        mDirection = dir;
        mNeedsUpdate = true;
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetCameraCorner(Corners corner) const
    {
        return mCameraCorners[corner];
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }
};

} // namespace Scene
} // namespace lkRay
