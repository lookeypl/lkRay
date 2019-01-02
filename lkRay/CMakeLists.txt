PKG_CHECK_MODULES(LKRAY_PKG_DEPS REQUIRED
                  xcb xcb-image
                  )

SET(LKRAY_SRCS PCH.cpp
               Main.cpp
               Distribution/Function.cpp
               Distribution/Lambertian.cpp
               Distribution/SpecularReflection.cpp
               Distribution/SurfaceDistribution.cpp
               Geometry/Mesh.cpp
               Geometry/Primitive.cpp
               Geometry/Ray.cpp
               Geometry/Sphere.cpp
               Geometry/Triangle.cpp
               Material/Material.cpp
               Material/Matte.cpp
               Material/Mirror.cpp
               Renderer/Renderer.cpp
               Scene/Camera.cpp
               Scene/Light.cpp
               Scene/RayCollision.cpp
               Scene/Scene.cpp
               )

SET(LKRAY_HDRS PCH.hpp
               Distribution/Function.hpp
               Distribution/Lambertian.hpp
               Distribution/SpecularReflection.hpp
               Distribution/SurfaceDistribution.hpp
               Geometry/Mesh.hpp
               Geometry/Primitive.hpp
               Geometry/Ray.hpp
               Geometry/Sphere.hpp
               Geometry/Triangle.hpp
               Material/Material.hpp
               Material/Matte.hpp
               Material/Mirror.hpp
               Renderer/Renderer.hpp
               Scene/Camera.hpp
               Scene/Light.hpp
               Scene/RayCollision.hpp
               Scene/Scene.hpp
               )

ADD_EXECUTABLE(${LKRAY_TARGET}
               ${LKRAY_SRCS} ${LKRAY_HDRS}
               )

ADD_DEPENDENCIES(${LKRAY_TARGET} lkCommon)

TARGET_INCLUDE_DIRECTORIES(${LKRAY_TARGET}
                           PRIVATE ${LKRAY_ROOT_DIRECTORY}/Deps/lkCommon/lkCommon/include
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

TARGET_LINK_LIBRARIES(${LKRAY_TARGET} lkCommon ${LKRAY_PKG_DEPS_LIBRARIES})

LKRAY_COPY_RESULT(${LKRAY_TARGET})
