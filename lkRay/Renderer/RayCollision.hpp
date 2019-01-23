#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>

#include "Prerequisites.hpp"


namespace lkRay {
namespace Renderer {

// gathers information about current collision point
class RayCollision
{
public:
    int32_t mHitID;
    float mDistance;
    lkCommon::Math::Vector4 mPoint;
    lkCommon::Math::Vector4 mNormal;

    // to be allocated at a later point
    lkCommon::Utils::ArenaAllocator* mAllocator;
    Renderer::SurfaceDistribution* mSurfaceDistribution;


    RayCollision();
    RayCollision(int hitID, float distance, const lkCommon::Math::Vector4& point, const lkCommon::Math::Vector4& normal);
    ~RayCollision();
};

} // namespace Renderer
} // namespace lkRay
