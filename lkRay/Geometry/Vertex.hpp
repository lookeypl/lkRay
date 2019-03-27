#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <vector>

#include "UV.hpp"


namespace lkRay {
namespace Geometry {

struct Vertex
{
    lkCommon::Math::Vector4 p;
    lkCommon::Math::Vector4 n;
    UV t;
};

using Vertices = std::vector<Vertex>;

} // namespace Geometry
} // namespace lkRay
