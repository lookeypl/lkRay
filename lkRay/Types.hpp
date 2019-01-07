#pragma once

namespace lkRay {
namespace Types {

enum class Light: unsigned char
{
    UNKNOWN = 0,
    POINT,
};

enum class Material: unsigned char
{
    UNKNOWN = 0,
    MATTE,
    MIRROR,
};

enum class Primitive: unsigned char
{
    UNKNOWN = 0,
    SPHERE,
    PLANE,
    MESH
};

} // namespace lkRay
} // namespace Types
