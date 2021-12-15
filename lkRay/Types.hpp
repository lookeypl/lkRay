#pragma once

#include <lkCommon/lkCommon.hpp>


namespace lkRay {
namespace Types {

enum class Light: unsigned char
{
    UNKNOWN = 0,
    POINT,
    DIR,
    SPOT,
};

enum class Material: unsigned char
{
    UNKNOWN = 0,
    MATTE,
    MIRROR,
    EMISSIVE,
};

enum class Primitive: unsigned char
{
    UNKNOWN = 0,
    SPHERE,
    PLANE,
    MESH,
};

enum class Distribution: unsigned char
{
    NONE = 0,
    REFLECTION = 1 << 0,
    TRANSMISSION = 1 << 1,
    DIFFUSE = 1 << 2,
    GLOSS = 1 << 3,
    SPECULAR = 1 << 4,
    EMISSIVE = 1 << 5,
};

LKCOMMON_INLINE Distribution operator&(const Distribution a, const Distribution b)
{
    return static_cast<Distribution>(
        static_cast<std::underlying_type<Distribution>::type>(a) &
        static_cast<std::underlying_type<Distribution>::type>(b)
    );
}

LKCOMMON_INLINE Distribution operator|(const Distribution a, const Distribution b)
{
    return static_cast<Distribution>(
        static_cast<std::underlying_type<Distribution>::type>(a) |
        static_cast<std::underlying_type<Distribution>::type>(b)
    );
}

} // namespace lkRay
} // namespace Types
