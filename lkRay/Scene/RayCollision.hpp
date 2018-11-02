#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>

#include "Material/SurfaceDistribution.hpp"


namespace lkRay {
namespace Scene {

// gathers information about current collision point
class RayCollision
{
public:
    int32_t mHitID;
    float mDistance;
    lkCommon::Math::Vector4 mCollisionPoint;
    lkCommon::Math::Vector4 mCollisionNormal;

    // to be allocated at a later point
    lkCommon::Utils::ArenaAllocator* mAllocator;
    Material::SurfaceDistribution* mSurfaceDistribution;


    RayCollision();
    RayCollision(int hitID, float distance, const lkCommon::Math::Vector4& collisionPoint, const lkCommon::Math::Vector4& normal);
    ~RayCollision();
};

} // namespace Scene
} // namespace lkRay
