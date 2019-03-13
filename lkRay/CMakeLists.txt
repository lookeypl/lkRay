PKG_CHECK_MODULES(LKRAY_PKG_DEPS REQUIRED
                  xcb xcb-image
                  )

SET(LKRAY_SRCS PCH.cpp
               Constants.cpp
               Main.cpp
               Distribution/Function.cpp
               Distribution/Lambertian.cpp
               Distribution/SpecularReflection.cpp
               Geometry/AABB.cpp
               Geometry/Mesh.cpp
               Geometry/Plane.cpp
               Geometry/Primitive.cpp
               Geometry/Ray.cpp
               Geometry/Sphere.cpp
               Geometry/Triangle.cpp
               Light/DirLight.cpp
               Light/Light.cpp
               Light/PointLight.cpp
               Light/SpotLight.cpp
               Material/Material.cpp
               Material/Matte.cpp
               Material/Mirror.cpp
               Renderer/PathContext.cpp
               Renderer/RayCollision.cpp
               Renderer/Renderer.cpp
               Renderer/SurfaceDistribution.cpp
               Renderer/ThreadData.cpp
               Scene/Camera.cpp
               Scene/Containers.cpp
               Scene/Scene.cpp
               )

SET(LKRAY_HDRS PCH.hpp
               Constants.hpp
               Distribution/Function.hpp
               Distribution/Lambertian.hpp
               Distribution/SpecularReflection.hpp
               Geometry/AABB.hpp
               Geometry/Mesh.hpp
               Geometry/Plane.hpp
               Geometry/Primitive.hpp
               Geometry/Ray.hpp
               Geometry/Sphere.hpp
               Geometry/Triangle.hpp
               Light/DirLight.hpp
               Light/Light.hpp
               Light/PointLight.hpp
               Light/SpotLight.hpp
               Material/Material.hpp
               Material/Matte.hpp
               Material/Mirror.hpp
               Renderer/PathContext.hpp
               Renderer/RayCollision.hpp
               Renderer/Renderer.hpp
               Renderer/SurfaceDistribution.hpp
               Renderer/ThreadData.hpp
               Scene/Camera.hpp
               Scene/Containers.hpp
               Scene/Scene.hpp
               )

ADD_EXECUTABLE(${LKRAY_TARGET}
               ${LKRAY_SRCS} ${LKRAY_HDRS}
               )

ADD_DEPENDENCIES(${LKRAY_TARGET} lkCommon)

TARGET_INCLUDE_DIRECTORIES(${LKRAY_TARGET}
                           PRIVATE ${LKRAY_ROOT_DIRECTORY}/Deps/lkCommon/lkCommon/include
                           PRIVATE ${LKRAY_ROOT_DIRECTORY}/Deps/rapidjson/include
                           PRIVATE ${LKRAY_ROOT_DIRECTORY}/Deps/tinyobjloader
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

TARGET_COMPILE_DEFINITIONS(${LKRAY_TARGET}
                           PRIVATE "-DLKRAY_ROOT_REL_TO_BIN=\"../..\""
                           PRIVATE "-DLKRAY_ROOT_DIR=R\"(${LKRAY_ROOT_DIRECTORY}/)\""
                           )

TARGET_LINK_LIBRARIES(${LKRAY_TARGET} lkCommon ${LKRAY_PKG_DEPS_LIBRARIES})

LKRAY_COPY_RESULT(${LKRAY_TARGET})
