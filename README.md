lkRay
=====

lkRay is an experimental path tracer application.

This project is done mostly for fun and to explore world of some cool graphics rendering. It's a hobby after-hours project, so new features will come at their own time.

Released under WTFPL license.

Features
--------

* Multithreaded rendering of scenes using path tracing
* Scenes and meshes accelerated using Bounding Volume Hierarchy
* Support for primitives:
  * Sphere
  * Plane
  * Mesh (a set of triangles)
* Support for light sources:
  * Point
  * Spot
  * Directional
* Brief support for PBR materials:
  * Matte material
  * Mirror material
* Scene loading using JSON files
* Post processing & gamma correction

Scenes
------

Following scenes are available in `Data/Scenes` folder:

1. `balls.json` - Basic empty scene with 8 spheres.
1. `emptyplane.json` - Scene itself contains only a plane and a directional light. However, in Main.cpp code, scene is filled with 1000 random spheres used for Scene BVH testing.
1. `room.json` - Closed room with some spheres and a spot light.
1. `plane.json` - Open space with walls, spheres and a directional light.
1. `mirrors.json` - "Hall of mirrors" effect scene.

Controls
--------

Control list is as follows:

* Hold `LMB` to activate FPP camera movement:
  * `W/S/A/D/R/F` to move forward/backward/left/right/up/down
  * Mouse move to rotate
* `Z/X` - Decrease/increase camera exposure
* `C/V` - Decrease/increase ray bounce limit
* `P` - Print current camera info on console
* `1-5` - Change current scene

Camera in lkRay works in two modes.

When `LMB` is pressed, camera is free to move around. Then, results of rendering are discarded each pass and ray depth is limited to no bounces to allow somewhat-close-to-realtime control over camera.

When `LMB` is not pressed, app is in rendering mode. Then, each pass is accumulated to achieve a better quality image.

Dependencies
------------

Buildable dependencies of lkRay is only my [commons library](https://github.com/lkostyra/lkCommon) for most of the code (partially lkRay is also my test field for most of the functionalities in lkCommon, so that dependency can be updated quite often). Other libraries include [rapidjson](https://github.com/Tencent/rapidjson) and [tinyobjloader](https://github.com/syoyo/tinyobjloader). All are acquirable through git submodules.

Both rapidjson and tinyobjloader are header-only, so they do not require rebuilding. lkCommon is included in builds both inside Visual Studio solution and through CMake build, so all dependencies should build together with main project.

Building for Windows
--------------------

Fetch submodules and use provided lkCommon.sln solution file, compatible with Visual Studio 2017. CMake-generated build files were never tested if they work properly under Windows.

Building for Linux
------------------

Fetch submodules and use cmake/make to build. Minimum requirement is CMake 2.6, GCC supporting C++14 and dependencies of lkCommon (see [its README](https://github.com/lkostyra/lkCommon#dependencies) for more info).
