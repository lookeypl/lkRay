#pragma once

#include <string>


namespace lkRay {
namespace Constants {

extern const std::string LKRAY_DEFAULT_MATERIAL_NAME;


// JSON parsing names
extern const std::string NAME_NODE_NAME;
extern const std::string AMBIENT_NODE_NAME;

// materials
extern const std::string MATERIALS_NODE_NAME;
extern const std::string MATERIAL_MATTE_NODE_NAME;
extern const std::string MATERIAL_MIRROR_NODE_NAME;

extern const std::string MATERIAL_ATTRIBUTE_TYPE_NODE_NAME;

// lights
extern const std::string LIGHTS_NODE_NAME;
extern const std::string LIGHT_POINT_NODE_NAME;

extern const std::string LIGHT_ATTRIBUTE_TYPE_NODE_NAME;
extern const std::string LIGHT_ATTRIBUTE_POSITION_NODE_NAME;
extern const std::string LIGHT_ATTRIBUTE_COLOR_NODE_NAME;

extern const std::string POINT_LIGHT_ATTRIBUTE_ATTENUATION_FACTOR_NODE_NAME;

// objects
extern const std::string OBJECTS_NODE_NAME;
extern const std::string OBJECT_SPHERE_NODE_NAME;
extern const std::string OBJECT_PLANE_NODE_NAME;
extern const std::string OBJECT_MESH_NODE_NAME;

extern const std::string OBJECT_ATTRIBUTE_TYPE_NODE_NAME;
extern const std::string OBJECT_ATTRIBUTE_POSITION_NODE_NAME;
extern const std::string OBJECT_ATTRIBUTE_MATERIAL_NODE_NAME;

extern const std::string SPHERE_ATTRIBUTE_RADIUS_NODE_NAME;

extern const std::string MESH_ATTRIBUTE_VERTICES_NODE_NAME;
extern const std::string MESH_ATTRIBUTE_INDICES_NODE_NAME;

} // namespace lkRay
} // namespace Constants
