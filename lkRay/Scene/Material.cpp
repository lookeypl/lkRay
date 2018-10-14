#include "PCH.hpp"
#include "Material.hpp"


namespace lkRay {
namespace Scene {

Material::Material()
    : mColor(1.0f)
    , mReflection(0.0f)
    , mAlbedo(1.0f)
    , mTransparency(0.0f)
{
}

} // namespace Scene
} // namespace lkRay
