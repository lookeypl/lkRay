#include "PCH.hpp"
#include "Camera.hpp"

#include "Constants.hpp"

#include <lkCommon/Math/Constants.hpp>

#include <cmath>
#include <fstream>


namespace lkRay {
namespace Scene {

Camera::Camera(const lkCommon::Math::Vector4& pos, const lkCommon::Math::Vector4& worldUp,
               const float angleAzim, const float anglePolar,
               const float horizontalFov, const float aspect)
    : mPosition(pos)
    , mForwardDir()
    , mSideDir()
    , mUpDir()
    , mWorldUp(worldUp.Normalize())
    , mHalfFovRad(LKCOMMON_DEG_TO_RADF(horizontalFov * 0.5f))
    , mAspectRatio(aspect)
    , mAnglePhi(LKCOMMON_DEG_TO_RADF(anglePolar))
    , mAngleTheta(LKCOMMON_DEG_TO_RADF(angleAzim))
    , mNeedsUpdate(true)
{
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
    ).Normalize();

    // calculate rest of Camera's axes
    mSideDir = mWorldUp.Cross(mForwardDir).Normalize();
    mUpDir = mForwardDir.Cross(mSideDir).Normalize();
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

bool Camera::ReadParametersFromNode(const rapidjson::Value& value)
{
    for (auto& a: value.GetObject())
    {
        if (Constants::CAMERA_ATTRIBUTE_POSITION_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Invalid normal parameter for camera. Should be an array of 3 floats.");
                return false;
            }

            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mPosition[colIndex] = c.GetFloat();
                colIndex++;
            }

            mPosition[3] = 1.0f;

            LOGD(" -> Camera's position " << mPosition);
        }
        else if (Constants::CAMERA_ATTRIBUTE_ROTATION_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 2))
            {
                LOGE("Invalid rotation parameter for camera. Should be an array of 2 floats.");
                return false;
            }

            float angles[2] = { 0.0f, 0.0f };
            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                angles[colIndex] = c.GetFloat();
                colIndex++;
            }

            mAngleTheta = LKCOMMON_DEG_TO_RADF(angles[0]);
            mAnglePhi = LKCOMMON_DEG_TO_RADF(angles[1]);
            LOGD(" -> Camera's rotation " << mAngleTheta << ", " << mAnglePhi);
        }
    }

    mNeedsUpdate = true;
    Update();
    return true;
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
    mAngleTheta -= angleRad;

    // limit values
    if (mAngleTheta > (LKCOMMON_PIF / 2.0f) - 0.1f)
        mAngleTheta = (LKCOMMON_PIF / 2.0f) - 0.1f;
    if (mAngleTheta < (-LKCOMMON_PIF / 2.0f) + 0.1f)
        mAngleTheta = (-LKCOMMON_PIF / 2.0f) + 0.1f;

    mNeedsUpdate = true;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    mAspectRatio = aspectRatio;
    UpdateCorners();
}

bool Camera::ReadParametersFromScene(const std::string& path)
{
    // load scene from JSON
    std::fstream sceneFile(path, std::fstream::in);
    if (!sceneFile)
    {
        LOGE("Failed to open scene file " << path);
        return false;
    }

    std::string sceneFileString((std::istreambuf_iterator<char>(sceneFile)),
                                std::istreambuf_iterator<char>());

    sceneFile.close();

    rapidjson::Document sceneDesc;
    sceneDesc.Parse(sceneFileString.c_str());

    if (!sceneDesc.IsObject())
    {
        LOGE("File is not a proper JSON object");
        return false;
    }

    // parse camera's name and ambient color
    for (auto& o: sceneDesc.GetObject())
    {
        if (Constants::CAMERA_NODE_NAME.compare(o.name.GetString()) == 0)
        {
            return ReadParametersFromNode(o.value);
        }
    }

    // no camera node found - just return true and don't change a thing
    return true;
}

} // namespace Scene
} // namespace lkRay
