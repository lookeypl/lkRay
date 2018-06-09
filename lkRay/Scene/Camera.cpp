#include "PCH.hpp"
#include "Camera.hpp"

#include <lkCommon/Math/Constants.hpp>


namespace lkRay {
namespace Scene {

Camera::Camera(const lkCommon::Math::Vector4& pos, const lkCommon::Math::Vector4& dir, const lkCommon::Math::Vector4& up, const float horizontalFov, const float aspect)
    : mPosition(pos)
    , mDirection(dir)
    , mUp(up)
    , mHalfFovRad(LKCOMMON_DEG_TO_RADF(horizontalFov))
    , mAspectRatio(aspect)
    , mNeedsUpdate(true)
{
    mDirection.Normalize();
    UpdateCorners();
}

// TODO pass information about updated corners to Renderer
void Camera::UpdateCorners()
{
    if (!mNeedsUpdate)
        return;

    mNeedsUpdate = false;

    // first, grab camera's axes, assuming mDirection is a Z axis
    lkCommon::Math::Vector4 xAxis = mUp;
    xAxis.Cross(mDirection);
    xAxis.Normalize();
    lkCommon::Math::Vector4 yAxis = mDirection;
    yAxis.Cross(xAxis);
    yAxis.Normalize();

    // assume we calculate image's plane 1.0f away from Camera's origin
    // the distance doesn't actually matter, it just makes the calculations easier
    float halfWidth = tanf(mHalfFovRad);
    float halfHeight = halfWidth / mAspectRatio;

    mCameraCorners[Corners::TOP_L] = mPosition + mDirection + yAxis * halfHeight - xAxis * halfWidth;
    mCameraCorners[Corners::TOP_R] = mPosition + mDirection + yAxis * halfHeight + xAxis * halfWidth;
    mCameraCorners[Corners::BOT_L] = mPosition + mDirection - yAxis * halfHeight - xAxis * halfWidth;
    mCameraCorners[Corners::BOT_R] = mPosition + mDirection - yAxis * halfHeight + xAxis * halfWidth;
}

} // namespace Scene
} // namespace lkRay
