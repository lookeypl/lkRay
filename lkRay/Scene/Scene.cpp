#include "PCH.hpp"
#include "Scene.hpp"

#include "Geometry/Primitive.hpp"

#include <limits>


namespace lkRay {
namespace Scene {

Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::Load(const std::string& path)
{
    // TODO
    return false;
}

void Scene::AddPrimitive(const std::shared_ptr<Geometry::Primitive>& ptr)
{
    mPrimitives.push_back(ptr);
}


void Scene::AddLight(const Light::Ptr& ptr)
{
    mLights.push_back(ptr);
}

lkCommon::Utils::PixelFloat4 Scene::SampleLights(const RayCollision& collision) const
{
    lkCommon::Utils::PixelFloat4 result;

    for (const auto& l: mLights)
    {
        lkCommon::Math::Vector4 lightDir = l->GetPosition() - collision.mCollisionPoint;
        Geometry::Ray shadowRay(collision.mCollisionPoint, lightDir.Normalize());
        RayCollision shadowCollision = TestCollision(shadowRay, collision.mHitID);
        // if shadow ray did not hit anything, or it hit an object which is further from light 
        if (shadowCollision.mHitID == -1 || shadowCollision.mDistance > lightDir.Length())
            result += l->Sample(collision);
    }

    return result;
}

// -1 if no object hit, otherwise index of hit primitive
RayCollision Scene::TestCollision(const Geometry::Ray& ray, int skipObjID) const
{
    int32_t hitID = -1;
    float colDistance = std::numeric_limits<float>::max();
    lkCommon::Math::Vector4 colNormal;
    float testDistance = 0.0f;
    lkCommon::Math::Vector4 testNormal;

    for (size_t i = 0; i < mPrimitives.size(); ++i)
    {
        if (skipObjID == i)
            continue;

        if (mPrimitives[i]->TestCollision(ray, testDistance, testNormal) &&
            testDistance < colDistance)
        {
            colDistance = testDistance;
            colNormal = testNormal;
            hitID = static_cast<int32_t>(i);
        }
    }

    return RayCollision(hitID, colDistance, ray.GetOrigin() + ray.GetDirection() * colDistance, colNormal);
}

} // namespace Scene
} // namespace lkRay
