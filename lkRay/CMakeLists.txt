PKG_CHECK_MODULES(LKRAY_PKG_DEPS REQUIRED
                  xcb
                  )

SET(LKRAY_SRCS PCH.cpp
               Main.cpp
               Geometry/Primitive.cpp
               Geometry/Sphere.cpp
               )

SET(LKRAY_HDRS PCH.hpp
               Geometry/Primitive.hpp
               Geometry/Sphere.hpp
               )

ADD_EXECUTABLE(${LKRAY_TARGET}
               ${LKRAY_SRCS} ${LKRAY_HDRS}
               )

ADD_DEPENDENCIES(${LKRAY_TARGET} lkCommon)

TARGET_INCLUDE_DIRECTORIES(${LKRAY_TARGET}
                           PRIVATE ${LKRAY_ROOT_DIRECTORY}/Deps/lkCommon)

TARGET_LINK_LIBRARIES(${LKRAY_TARGET} lkCommon ${LKRAY_PKG_DEPS_LIBRARIES})

LKRAY_COPY_RESULT(${LKRAY_TARGET})
