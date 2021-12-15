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
using Container = std::vector<T>;

using Primitive = Container<Ptr<Geometry::Primitive>>;
using Material = std::map<std::string, Ptr<Material::Material>>;

} // namespace lkRay
} // namespace Scene
} // namespace Containers
