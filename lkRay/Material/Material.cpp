#include "PCH.hpp"
#include "Material.hpp"


namespace lkRay {
namespace Material {

Material::Material()
    : mType(MaterialType::UNKNOWN)
{
}

Material::Material(MaterialType type)
    : mType(type)
{
}

} // namespace Material
} // namespace lkRay
