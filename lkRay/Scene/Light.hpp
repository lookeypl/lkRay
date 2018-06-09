#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <memory>

namespace lkRay {
namespace Scene {

class Light
{
public:
    using Ptr = std::shared_ptr<Light>;

private:
    lkCommon::Math::Vector4 mPosition;

public:
    Light(const lkCommon::Math::Vector4& pos);
    ~Light() = default;

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }
};

} // namespace Scene
} // namespace lkRay
