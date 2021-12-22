#include "PCH.hpp"
#include "Constants.hpp"


namespace lkRay {
namespace Constants {


const std::string LKRAY_DEFAULT_MATERIAL_NAME = "LKRAY_DEFAULT_MAT";


// JSON parsing names
const std::string NAME_NODE_NAME = "name";
const std::string AMBIENT_NODE_NAME = "ambient";
const std::string CAMERA_NODE_NAME = "camera";

// materials
const std::string MATERIALS_NODE_NAME = "materials";
const std::string MATERIAL_MATTE_NODE_NAME = "matte";
const std::string MATERIAL_MIRROR_NODE_NAME = "mirror";
const std::string MATERIAL_EMISSIVE_NODE_NAME = "emissive";
const std::string MATERIAL_GLASS_NODE_NAME = "glass";

const std::string MATERIAL_ATTRIBUTE_TYPE_NODE_NAME = "type";

// lights
const std::string LIGHTS_NODE_NAME = "lights";
const std::string LIGHT_POINT_NODE_NAME = "point";
const std::string LIGHT_DIR_NODE_NAME = "dir";
const std::string LIGHT_SPOT_NODE_NAME = "spot";

const std::string LIGHT_ATTRIBUTE_TYPE_NODE_NAME = "type";
const std::string LIGHT_ATTRIBUTE_POSITION_NODE_NAME = "position";
const std::string LIGHT_ATTRIBUTE_COLOR_NODE_NAME = "color";

const std::string POINT_LIGHT_ATTRIBUTE_ATTENUATION_FACTOR_NODE_NAME = "attFactor";

const std::string DIR_LIGHT_ATTRIBUTE_DIR_NODE_NAME = "direction";

const std::string SPOT_LIGHT_ATTRIBUTE_ATTENUATION_FACTOR_NODE_NAME = "attFactor";
const std::string SPOT_LIGHT_ATTRIBUTE_DIR_NODE_NAME = "direction";
const std::string SPOT_LIGHT_ATTRIBUTE_CUTOFF_NODE_NAME = "cutoff";

// objects
const std::string OBJECTS_NODE_NAME = "objects";
const std::string OBJECT_SPHERE_NODE_NAME = "sphere";
const std::string OBJECT_PLANE_NODE_NAME = "plane";
const std::string OBJECT_MESH_NODE_NAME = "mesh";

const std::string OBJECT_ATTRIBUTE_TYPE_NODE_NAME = "type";
const std::string OBJECT_ATTRIBUTE_POSITION_NODE_NAME = "position";
const std::string OBJECT_ATTRIBUTE_MATERIAL_NODE_NAME = "material";

const std::string SPHERE_ATTRIBUTE_RADIUS_NODE_NAME = "radius";

const std::string PLANE_ATTRIBUTE_NORMAL_NODE_NAME = "normal";
const std::string PLANE_ATTRIBUTE_DISTANCE_NODE_NAME = "distance";

const std::string MESH_ATTRIBUTE_PATH_NODE_NAME = "path";
const std::string MESH_ATTRIBUTE_VERTICES_NODE_NAME = "vertices";
const std::string MESH_ATTRIBUTE_INDICES_NODE_NAME = "indices";

// materials
const std::string MATTE_ATTRIBUTE_COLOR_NODE_NAME = "color";
const std::string MATTE_ATTRIBUTE_PATH_NODE_NAME = "path";
const std::string EMISSIVE_ATTRIBUTE_INTENSITY_NODE_NAME = "intensity";
const std::string GLASS_ATTRIBUTE_DENSITY_NODE_NAME = "density";

// camera
const std::string CAMERA_ATTRIBUTE_POSITION_NODE_NAME = "position";
const std::string CAMERA_ATTRIBUTE_ROTATION_NODE_NAME = "rotation";


} // namespace lkRay
} // namespace Constants
