#pragma once

#include <vector>
#include <memory>
#include <map>

#include "Prerequisites.hpp"


namespace lkRay {
namespace Scene {
namespace Containers {

template <typename T>
using Ptr = std::shared_ptr<T>;
template <typename T>
using Container = std::vector<Ptr<T>>;

using Primitive = Container<Geometry::Primitive>;
using Light = Container<Light::Light>;
using Material = std::map<std::string, Ptr<Material::Material>>;

} // namespace lkRay
} // namespace Scene
} // namespace Containers
