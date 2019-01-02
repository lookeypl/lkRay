#pragma once

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>

#include "Scene/Scene.hpp"
#include "Scene/RayCollision.hpp"
#include "Distribution/Function.hpp"


namespace lkRay {
namespace Material {

enum class MaterialType: unsigned char
{
    UNKNOWN = 0,
    MATTE,
    MIRROR,
};

class Material
{
protected:
    MaterialType mType;

public:
    Material();
    Material(MaterialType type);
    ~Material() = default;

    virtual void PopulateDistributionFunctions(Scene::RayCollision& collision) = 0;
};

} // namespace Material
} // namespace lkRay
