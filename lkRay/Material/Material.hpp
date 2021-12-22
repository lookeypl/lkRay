#pragma once

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Allocators/ArenaAllocator.hpp>

#include "Renderer/RayCollision.hpp"
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

    virtual void PopulateDistributionFunctions(Renderer::PathContext& context, Renderer::RayCollision& collision) = 0;
    virtual bool ReadParametersFromNode(const rapidjson::Value& value) = 0;
};

} // namespace Material
} // namespace lkRay
