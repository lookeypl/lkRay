#pragma once

#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/ArenaObject.hpp>
#include <type_traits>


namespace lkRay {
namespace Material {


enum class DistributionType: unsigned char
{
    NONE = 0,
    REFLECTION = 1 << 0,
    TRANSMISSION = 1 << 1,
    DIFFUSE = 1 << 2,
    GLOSS = 1 << 3,
    SPECULAR = 1 << 4,
    ALL = SPECULAR | GLOSS | DIFFUSE | TRANSMISSION | REFLECTION,
};

inline DistributionType operator&(const DistributionType a, const DistributionType b)
{
    return static_cast<DistributionType>(
        static_cast<std::underlying_type<DistributionType>::type>(a) &
        static_cast<std::underlying_type<DistributionType>::type>(b)
    );
}

inline DistributionType operator|(const DistributionType a, const DistributionType b)
{
    return static_cast<DistributionType>(
        static_cast<std::underlying_type<DistributionType>::type>(a) |
        static_cast<std::underlying_type<DistributionType>::type>(b)
    );
}

// Base class for methods of ray distribution on given surface
// gathers both Reflection and Transmission type of distribution
class Distribution: public lkCommon::Utils::ArenaObject
{
protected:
    const DistributionType mType;

public:
    LKCOMMON_INLINE Distribution(const DistributionType type)
        : mType(type)
    {}

    ~Distribution() = default;

    virtual lkCommon::Utils::PixelFloat4 F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out) = 0;

    LKCOMMON_INLINE bool HasType(const DistributionType type) const
    {
        return (mType & type) == type;
    }
};

} // namespace Material
} // namespace lkRay
