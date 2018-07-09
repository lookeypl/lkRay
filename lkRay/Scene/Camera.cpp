#include "PCH.hpp"
#include "Camera.hpp"

#include <lkCommon/Math/Constants.hpp>


namespace lkRay {
namespace Scene {

Camera::Camera(const lkCommon::Math::Vector4& pos, const lkCommon::Math::Vector4& worldUp,
               const float angleAzim, const float anglePolar,
               const float horizontalFov, const float aspect)
    : mPosition(pos)
    , mForwardDir()
    , mSideDir()
    , mUpDir()
    , mWorldUp(worldUp)
    , mHalfFovRad(LKCOMMON_DEG_TO_RADF(horizontalFov * 0.5f))
    , mAspectRatio(aspect)
    , mAnglePhi(LKCOMMON_DEG_TO_RADF(anglePolar))
    , mAngleTheta(LKCOMMON_DEG_TO_RADF(angleAzim))
    , mNeedsUpdate(true)
{
    mWorldUp.Normalize();
    CalculateCameraAxes();
    UpdateCorners();
}

void Camera::CalculateCameraAxes()
{
    // convert spehrical coordinates (angles) to cartesian
    mForwardDir = lkCommon::Math::Vector4(
        sinf(mAnglePhi) * cosf(mAngleTheta),
        sinf(mAngleTheta),
        cosf(mAnglePhi) * cosf(mAngleTheta),
        0.0f
    );
    mForwardDir.Normalize();

    // calculate rest of Camera's axes
    mSideDir = mWorldUp;
    mSideDir.Cross(mForwardDir);
    mSideDir.Normalize();

    mUpDir = mForwardDir;
    mUpDir.Cross(mSideDir);
    mUpDir.Normalize();
}

void Camera::UpdateCorners()
{
    // assume we calculate image's plane 1.0f away from Camera's origin
    // the distance doesn't actually matter, it just simplifies the calculations
    float halfWidth = tanf(mHalfFovRad);
    float halfHeight = halfWidth / mAspectRatio;

    mCameraCorners[Corners::TOP_L] = mPosition + mForwardDir + mUpDir * halfHeight - mSideDir * halfWidth;
    mCameraCorners[Corners::TOP_R] = mPosition + mForwardDir + mUpDir * halfHeight + mSideDir * halfWidth;
    mCameraCorners[Corners::BOT_L] = mPosition + mForwardDir - mUpDir * halfHeight - mSideDir * halfWidth;
    mCameraCorners[Corners::BOT_R] = mPosition + mForwardDir - mUpDir * halfHeight + mSideDir * halfWidth;
}

void Camera::Update()
{
    if (mNeedsUpdate)
    {
        CalculateCameraAxes();
        UpdateCorners();
        mNeedsUpdate = false;
    }
}

void Camera::MoveForward(float distance)
{
    mPosition += mForwardDir * distance;
    mNeedsUpdate = true;
}

void Camera::MoveSideways(float distance)
{
    mPosition += mSideDir * distance;
    mNeedsUpdate = true;
}

void Camera::MoveUp(float distance)
{
    mPosition += mUpDir * distance;
    mNeedsUpdate = true;
}

void Camera::MoveWorldUp(float distance)
{
    mPosition += mWorldUp * distance;
    mNeedsUpdate = true;
}

void Camera::RotateLeftRight(float angleRad)
{
    mAnglePhi += angleRad;

    // wrap values
    if (mAnglePhi > 2 * LKCOMMON_PIF)
        mAnglePhi -= 2 * LKCOMMON_PIF;
    if (mAnglePhi < 0.0f)
        mAnglePhi += 2 * LKCOMMON_PIF;

    mNeedsUpdate = true;
}

void Camera::RotateUpDown(float angleRad)
{
    mAngleTheta += angleRad;

    // limit values
    if (mAngleTheta > (LKCOMMON_PIF / 2.0f) - 0.1f)
        mAngleTheta = (LKCOMMON_PIF / 2.0f) - 0.1f;
    if (mAngleTheta < (-LKCOMMON_PIF / 2.0f) + 0.1f)
        mAngleTheta = (-LKCOMMON_PIF / 2.0f) + 0.1f;

    mNeedsUpdate = true;
}

} // namespace Scene
} // namespace lkRay
