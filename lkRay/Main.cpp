#include "PCH.hpp"

#include <array>

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
const uint32_t DEFAULT_SCENE = 2;
const float EXPOSURE_DEFAULT = 1.0f;
const float EXPOSURE_STEP = 0.1f;

using namespace lkRay;

lkCommon::Math::RingAverage<float, 20> gFrameTime;

const std::array<std::string, 4> SCENE_CONTAINER = {
    "Data/Scenes/balls.json",
    "Data/Scenes/room.json",
    "Data/Scenes/plane.json",
    "Data/Scenes/mirrors.json",
};


class lkRayWindow: public lkCommon::System::Window
{
    Renderer::Renderer& mRenderer;
    Scene::Camera& mCamera;
    float mExposure;
    uint32_t mCurrentScene;
    uint32_t mRayDepthRendering;
    Scene::Scene mScene;

protected:
    void OnKeyDown(const lkCommon::System::KeyCode key) override
    {
        uint32_t newScene = std::numeric_limits<uint32_t>::max();

        if (static_cast<int>(lkCommon::System::KeyCode::Num1) <= static_cast<int>(key) &&
            static_cast<int>(key) <= static_cast<int>(lkCommon::System::KeyCode::Num4))
        {
            newScene = static_cast<int>(key) - static_cast<int>(lkCommon::System::KeyCode::Num1);

            if (newScene != std::numeric_limits<uint32_t>::max())
            {
                if (!LoadScene(newScene))
                {
                    LOGE("Loading new scene failed - reloading previous one");
                    if (!LoadScene(mCurrentScene))
                    {
                        LOGE("Failed to reload previous scene after error. Leaving empty.");
                        return;
                    }
                }

                mCurrentScene = newScene;
                mRenderer.ResetImageBuffer();
            }
        }

        if (key == lkCommon::System::KeyCode::C)
        {
            if (mRayDepthRendering > 1)
                mRayDepthRendering--;

            mRenderer.SetMaxRayDepth(mRayDepthRendering);
            mRenderer.ResetImageBuffer();
        }

        if (key == lkCommon::System::KeyCode::V)
        {
            mRayDepthRendering++;
            mRenderer.SetMaxRayDepth(mRayDepthRendering);
            mRenderer.ResetImageBuffer();
        }

        if (IsKeyPressed(lkCommon::System::KeyCode::Z))
        {
            if (mExposure > EXPOSURE_STEP)
                mExposure -= EXPOSURE_STEP;
            mRenderer.SetExposure(mExposure);
        }

        if (IsKeyPressed(lkCommon::System::KeyCode::X))
        {
            mExposure += EXPOSURE_STEP;
            mRenderer.SetExposure(mExposure);
        }
    }

    void OnUpdate(const float deltaTime) override
    {
        SetTitle("lkRay - " +
                 std::to_string(mRenderer.GetSampleCount()) + " passes, " +
                 std::to_string(mRenderer.GetRayDepth()) + " depth, " +
                 std::to_string(gFrameTime.Get() * 1000.0f) + " ms avg");

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
    }

    void OnMouseMove(const uint32_t x, const uint32_t y, const int32_t deltaX, const int32_t deltaY) override
    {
        if (IsMouseKeyPressed(0))
        {
            float shiftX = static_cast<float>(deltaX) * 0.005f;
            float shiftY = static_cast<float>(deltaY) * 0.005f;

            mCamera.RotateLeftRight(shiftX);
            mCamera.RotateUpDown(shiftY);
        }
    }

    void OnMouseDown(const uint32_t) override
    {
        mRenderer.SetMaxRayDepth(MAX_RAY_DEPTH_MOVEMENT);
    }

    void OnMouseUp(const uint32_t) override
    {
        mRenderer.SetMaxRayDepth(mRayDepthRendering);
    }

    void OnResize(const uint32_t width, const uint32_t height) override
    {
        mRenderer.ResizeOutput(width, height);
        mCamera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }

    bool OnOpen() override
    {
        if (!LoadScene(mCurrentScene))
        {
            LOGE("Failed to load scene " << SCENE_CONTAINER[mCurrentScene]);
            return false;
        }

        return true;
    }

public:
    lkRayWindow(Renderer::Renderer& renderer, Scene::Camera& camera)
        : mRenderer(renderer)
        , mCamera(camera)
        , mExposure(EXPOSURE_DEFAULT)
        , mCurrentScene(DEFAULT_SCENE)
        , mRayDepthRendering(MAX_RAY_DEPTH_RENDERING)
        , mScene()
    {
    }

    bool LoadScene(const uint32_t sceneNumber)
    {
        if (sceneNumber > SCENE_CONTAINER.size())
        {
            LOGE("Invalid scene number");
            return false;
        }

        mScene.Destroy();

        if (!mScene.Load(SCENE_CONTAINER[sceneNumber]))
        {
            LOGE("Failed to load scene " << SCENE_CONTAINER[sceneNumber]);
            return false;
        }

        return true;
    }

    LKCOMMON_INLINE const Scene::Scene& GetScene() const
    {
        return mScene;
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


    lkCommon::Utils::Timer t;
    t.Start();
    while (gWindow.IsOpened())
    {
        float time = static_cast<float>(t.Stop());
        t.Start();

        if (time > 0.001f)
            gFrameTime.Add(time);

        gWindow.Update(time);
        renderer.Draw(gWindow.GetScene(), camera);
        gWindow.DisplayImage(0, 0, renderer.GetOutput().GetWindowImage());
    }

    gWindow.Close();
    return 0;
}
