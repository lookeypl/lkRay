#include "PCH.hpp"
#include "Material.hpp"


namespace lkRay {
namespace Material {

Material::Material()
    : mType(Types::Material::UNKNOWN)
{
}

Material::Material(Types::Material type)
    : mType(type)
{
}

} // namespace Material
} // namespace lkRay
