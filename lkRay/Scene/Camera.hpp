#pragma once

#include "lkCommon/Math/Vector4.hpp"

#include <rapidjson/document.h>


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
    lkCommon::Math::Vector4 mForwardDir;
    lkCommon::Math::Vector4 mSideDir;
    lkCommon::Math::Vector4 mUpDir;
    lkCommon::Math::Vector4 mWorldUp;
    lkCommon::Math::Vector4 mCameraCorners[4];
    float mHalfFovRad;
    float mAspectRatio;
    float mAnglePhi; // rotation along world Y axis
    float mAngleTheta; // rotation along camera's X axis
    bool mNeedsUpdate;

    void CalculateCameraAxes();
    void UpdateCorners();
    bool ReadParametersFromNode(const rapidjson::Value& value);

public:
    Camera(const lkCommon::Math::Vector4& pos, const lkCommon::Math::Vector4& worldUp,
           const float angleAzim, const float anglePolar,
           const float horizontalFov, const float aspect);
    ~Camera() = default;

    void Update();
    void MoveForward(float distance);
    void MoveSideways(float distance);
    void MoveUp(float distance);
    void MoveWorldUp(float distance);
    void RotateLeftRight(float angleRad);
    void RotateUpDown(float angleRad);
    void SetAspectRatio(float aspectRatio);
    bool ReadParametersFromScene(const std::string& path);

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetCameraCorner(Corners corner) const
    {
        return mCameraCorners[corner];
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }

    LKCOMMON_INLINE const float GetXRotation() const
    {
        return mAngleTheta;
    }

    LKCOMMON_INLINE const float GetYRotation() const
    {
        return mAnglePhi;
    }
};

} // namespace Scene
} // namespace lkRay
