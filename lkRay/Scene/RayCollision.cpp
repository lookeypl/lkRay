#include "PCH.hpp"
#include "RayCollision.hpp"

namespace lkRay {
namespace Scene {

RayCollision::RayCollision()
    : mHitID(-1)
    , mDistance(0.0f)
    , mCollisionPoint()
    , mCollisionNormal()
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
}

RayCollision::RayCollision(int hitID, float distance, const lkCommon::Math::Vector4& collisionPoint, const lkCommon::Math::Vector4& normal)
    : mHitID(hitID)
    , mDistance(distance)
    , mCollisionPoint(collisionPoint)
    , mCollisionNormal(normal)
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
}

RayCollision::~RayCollision()
{
    if (mAllocator && mSurfaceDistribution)
    {
        mSurfaceDistribution->~SurfaceDistribution();
        mAllocator->Free(mSurfaceDistribution);
    }
}

} // namespace Scene
} // namespace lkRay
