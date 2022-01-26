#pragma once

#include <string>


namespace lkRay {
namespace Constants {


extern const uint32_t LKRAY_RAY_DEPTH_LIMIT;

extern const std::string LKRAY_DEFAULT_MATERIAL_NAME;


// JSON parsing names
extern const std::string NAME_NODE_NAME;
extern const std::string AMBIENT_NODE_NAME;
extern const std::string CAMERA_NODE_NAME;

// materials
extern const std::string MATERIALS_NODE_NAME;
extern const std::string MATERIAL_MATTE_NODE_NAME;
extern const std::string MATERIAL_MIRROR_NODE_NAME;
extern const std::string MATERIAL_EMISSIVE_NODE_NAME;
extern const std::string MATERIAL_GLASS_NODE_NAME;

extern const std::string MATERIAL_ATTRIBUTE_TYPE_NODE_NAME;

// objects
extern const std::string OBJECTS_NODE_NAME;
extern const std::string OBJECT_SPHERE_NODE_NAME;
extern const std::string OBJECT_PLANE_NODE_NAME;
extern const std::string OBJECT_MESH_NODE_NAME;

extern const std::string OBJECT_ATTRIBUTE_TYPE_NODE_NAME;
extern const std::string OBJECT_ATTRIBUTE_POSITION_NODE_NAME;
extern const std::string OBJECT_ATTRIBUTE_MATERIAL_NODE_NAME;

extern const std::string SPHERE_ATTRIBUTE_RADIUS_NODE_NAME;

extern const std::string PLANE_ATTRIBUTE_NORMAL_NODE_NAME;
extern const std::string PLANE_ATTRIBUTE_DISTANCE_NODE_NAME;

extern const std::string MESH_ATTRIBUTE_PATH_NODE_NAME;
extern const std::string MESH_ATTRIBUTE_VERTICES_NODE_NAME;
extern const std::string MESH_ATTRIBUTE_INDICES_NODE_NAME;

// material attributes
extern const std::string MATTE_ATTRIBUTE_COLOR_NODE_NAME;
extern const std::string MATTE_ATTRIBUTE_PATH_NODE_NAME;
extern const std::string EMISSIVE_ATTRIBUTE_INTENSITY_NODE_NAME;
extern const std::string GLASS_ATTRIBUTE_DENSITY_NODE_NAME;

// camera
extern const std::string CAMERA_ATTRIBUTE_POSITION_NODE_NAME;
extern const std::string CAMERA_ATTRIBUTE_ROTATION_NODE_NAME;


} // namespace lkRay
} // namespace Constants
