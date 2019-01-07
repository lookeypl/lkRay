#include "PCH.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/Timer.hpp>
#include <lkCommon/System/Window.hpp>
#include <lkCommon/System/FS.hpp>
#include <lkCommon/Math/RingAverage.hpp>

#include "Renderer/Renderer.hpp"
#include "Scene/PointLight.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Geometry/Sphere.hpp"
#include "Geometry/Plane.hpp"
#include "Geometry/Mesh.hpp"
#include "Material/Matte.hpp"
#include "Material/Mirror.hpp"


// TODO This is a simple but pesky workaround to "find app root" issue.
//      Resolve this in the future by proper path searching, by one (or more) of:
//        * Going max 3 directories up, trying to find "Data" and "Bin" folder
//        * Searching for a predefined file hidden somewhere in "Data" dir?
#ifndef LKRAY_ROOT_REL_TO_BIN
#error "Path to root relative to binary not defined. Please define \
path to app's root as LKRAY_ROOT_REL_TO_BIN variable."
#endif


const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
const uint32_t MAX_RAY_DEPTH_MOVEMENT = 1;
const uint32_t MAX_RAY_DEPTH_RENDERING = 4;
const float EXPOSURE_DEFAULT = 0.6f;

using namespace lkRay;

lkCommon::Math::RingAverage<float, 20> gFrameTime;


class lkRayWindow: public lkCommon::System::Window
{
    Renderer::Renderer& mRenderer;
    Scene::Camera& mCamera;
    float mExposure = EXPOSURE_DEFAULT;

protected:
    void OnUpdate(float deltaTime) override
    {
        SetTitle("lkRay - " + std::to_string(mRenderer.GetSampleCount()) + " passes, " + std::to_string(gFrameTime.Get() * 1000.0f) + " ms avg");

        if (IsMouseKeyPressed(0))
        {
            mRenderer.ResetImageBuffer();

            float speed = 5.0f * deltaTime;
            if (IsKeyPressed(lkCommon::System::KeyCode::W)) mCamera.MoveForward(speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::S)) mCamera.MoveForward(-speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::D)) mCamera.MoveSideways(speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::A)) mCamera.MoveSideways(-speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::R)) mCamera.MoveWorldUp(speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::F)) mCamera.MoveWorldUp(-speed);

            mCamera.Update();
        }

        if (IsKeyPressed(lkCommon::System::KeyCode::Z))
        {
            mExposure -= 0.1f;
            mRenderer.SetExposure(mExposure);
        }

        if (IsKeyPressed(lkCommon::System::KeyCode::X))
        {
            mExposure += 0.1f;
            mRenderer.SetExposure(mExposure);
        }
    }

    void OnMouseMove(int x, int y, int deltaX, int deltaY) override
    {
        if (IsMouseKeyPressed(0))
        {
            float shiftX = static_cast<float>(deltaX) * 0.005f;
            float shiftY = static_cast<float>(deltaY) * 0.005f;

            mCamera.RotateLeftRight(shiftX);
            mCamera.RotateUpDown(-shiftY);
        }
    }

    void OnMouseDown(int) override
    {
        mRenderer.SetMaxRayDepth(MAX_RAY_DEPTH_MOVEMENT);
    }

    void OnMouseUp(int) override
    {
        mRenderer.SetMaxRayDepth(MAX_RAY_DEPTH_RENDERING);
    }

    void OnResize(int width, int height) override
    {
        mRenderer.ResizeOutput(width, height);
        mCamera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }

public:
    lkRayWindow(Renderer::Renderer& renderer, Scene::Camera& camera)
        : mRenderer(renderer)
        , mCamera(camera)
    {
    }
};


int main()
{
    if (!lkCommon::System::FS::SetCWD(
            lkCommon::System::FS::JoinPaths(
                lkCommon::System::FS::GetParentDir(lkCommon::System::FS::GetExecutablePath()),
                LKRAY_ROOT_REL_TO_BIN
                )
            )
        )
    {
        return -1;
    }

    Renderer::Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT, MAX_RAY_DEPTH_RENDERING);
    renderer.SetExposure(EXPOSURE_DEFAULT);

    Scene::Camera camera(
        lkCommon::Math::Vector4(0.0f, 1.0f, -7.0f, 1.0f),
        lkCommon::Math::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
        0.0f, 0.0f,
        75.0f, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)
    );

    lkRayWindow gWindow(renderer, camera);
    if (!gWindow.Init())
    {
        LOGE("Failed to initialize window");
        return -1;
    }

    if (!gWindow.Open(300, 300, WINDOW_WIDTH, WINDOW_HEIGHT, "lkRay"))
    {
        LOGE("Failed to open window");
        return -1;
    }


    Scene::Scene scene;

    if (!scene.Load("Data/Scenes/test.json"))
    {
        LOGE("Failed to load scene");
        return -1;
    }

    lkCommon::Utils::Timer t;
    t.Start();
    while (gWindow.IsOpen())
    {
        float time = static_cast<float>(t.Stop());
        t.Start();

        if (time > 0.001f)
            gFrameTime.Add(time);

        gWindow.Update(time);
        renderer.Draw(scene, camera);
        gWindow.DisplayImage(0, 0, renderer.GetOutput().GetWindowImage());
    }

    gWindow.Close();
    return 0;
}
