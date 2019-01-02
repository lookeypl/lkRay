#pragma once

#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/ArenaObject.hpp>
#include <type_traits>


namespace lkRay {
namespace Distribution {


enum class FunctionType: unsigned char
{
    NONE = 0,
    REFLECTION = 1 << 0,
    TRANSMISSION = 1 << 1,
    DIFFUSE = 1 << 2,
    GLOSS = 1 << 3,
    SPECULAR = 1 << 4,
    ALL = SPECULAR | GLOSS | DIFFUSE | TRANSMISSION | REFLECTION,
};

inline FunctionType operator&(const FunctionType a, const FunctionType b)
{
    return static_cast<FunctionType>(
        static_cast<std::underlying_type<FunctionType>::type>(a) &
        static_cast<std::underlying_type<FunctionType>::type>(b)
    );
}

inline FunctionType operator|(const FunctionType a, const FunctionType b)
{
    return static_cast<FunctionType>(
        static_cast<std::underlying_type<FunctionType>::type>(a) |
        static_cast<std::underlying_type<FunctionType>::type>(b)
    );
}

// Base class for methods of ray distribution on given surface
// gathers both Reflection and Transmission type of distribution
class Function: public lkCommon::Utils::ArenaObject
{
protected:
    const FunctionType mType;

public:
    LKCOMMON_INLINE Function(const FunctionType type)
        : mType(type)
    {}

    ~Function() = default;

    virtual lkCommon::Utils::PixelFloat4 F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out) = 0;

    LKCOMMON_INLINE bool HasType(const FunctionType type) const
    {
        return (mType & type) == type;
    }
};

} // namespace Distribution
} // namespace lkRay
