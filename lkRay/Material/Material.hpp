#pragma once

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>

#include "Scene/RayCollision.hpp"
#include "Distribution/Function.hpp"

#include <rapidjson/document.h>

#include <string>


namespace lkRay {
namespace Material {

class Material
{
protected:
    std::string mName;

public:
    Material(const std::string& name);
    ~Material() = default;

    virtual void PopulateDistributionFunctions(Scene::RayCollision& collision) = 0;
    virtual bool ReadParametersFromNode(const rapidjson::Value& value) = 0;
};

} // namespace Material
} // namespace lkRay
