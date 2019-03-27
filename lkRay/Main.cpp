#include "PCH.hpp"

#include <array>

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/Timer.hpp>
#include <lkCommon/System/Window.hpp>
#include <lkCommon/System/FS.hpp>
#include <lkCommon/Math/RingAverage.hpp>
#include <lkCommon/Math/Random.hpp>
#include <lkCommon/Utils/ArgParser.hpp>

#include "Renderer/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"

#include "Geometry/Sphere.hpp"
#include "Material/Matte.hpp"

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
const int32_t DEFAULT_SCENE = 0;
const int32_t DEFAULT_THREAD_COUNT = static_cast<int32_t>(lkCommon::System::Info::GetCPUCount());
const float EXPOSURE_DEFAULT = 1.0f;
const float EXPOSURE_STEP = 0.1f;
const float ADVERT_PERIOD = 5.0f;

using namespace lkRay;

lkCommon::Math::RingAverage<float, 20> gFrameTime;

const std::array<std::string, 6> SCENE_CONTAINER = {
    "Data/Scenes/balls.json",
    "Data/Scenes/emptyplane.json",
    "Data/Scenes/bunny.json",
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
    float mAdvertTimer;

protected:
    void OnKeyDown(const lkCommon::System::KeyCode key) override
    {
        uint32_t newScene = std::numeric_limits<uint32_t>::max();

        if (static_cast<int>(lkCommon::System::KeyCode::Num1) <= static_cast<int>(key) &&
            static_cast<int>(key) <= (static_cast<int>(lkCommon::System::KeyCode::Num1) + SCENE_CONTAINER.size()))
        {
            newScene = static_cast<int>(key) - static_cast<int>(lkCommon::System::KeyCode::Num1);

            if (newScene != std::numeric_limits<uint32_t>::max())
            {
                bool resetCamera = !IsKeyPressed(lkCommon::System::KeyCode::Shift);
                if (!LoadScene(newScene, resetCamera))
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

        if (IsKeyPressed(lkCommon::System::KeyCode::P))
        {
            LOGI("Current camera details:");
            LOGI("  Position: " << mCamera.GetPosition());
            LOGI("  Rotation: " <<
                 LKCOMMON_RAD_TO_DEG(mCamera.GetXRotation()) << " " <<
                 LKCOMMON_RAD_TO_DEG(mCamera.GetYRotation()));
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

        mAdvertTimer += deltaTime;
        if (mAdvertTimer > ADVERT_PERIOD)
        {
            LOGW(' ');
            LOGW("This is a free version of lkRay. To turn off adverts, please PayPal 10€ to lkostyra11@gmail.com");
            LOGW(' ');
            LOGW("!!! ADVERTISEMENT !!!");
            LOGW("Do you know that with Amazon Prime you also get Twitch Prime and a free subsctiption to any channel on Twitch?");
            LOGW("You can use it to, for example, support the best speedrunner Linkus7 by subscribing to him for free!");
            LOGW("  https://twitch.tv/linkus7");
            LOGW(' ');

            mAdvertTimer -= ADVERT_PERIOD;
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

public:
    lkRayWindow(Renderer::Renderer& renderer, Scene::Camera& camera)
        : mRenderer(renderer)
        , mCamera(camera)
        , mExposure(EXPOSURE_DEFAULT)
        , mCurrentScene(DEFAULT_SCENE)
        , mRayDepthRendering(MAX_RAY_DEPTH_RENDERING)
        , mScene()
        , mAdvertTimer(0.0f)
    {
    }

    bool LoadScene(const uint32_t sceneNumber, bool resetCamera = true)
    {
        if (sceneNumber > SCENE_CONTAINER.size())
        {
            LOGE("Invalid scene number");
            return false;
        }

        mScene.Destroy();

        if (resetCamera)
        {
            if (!mCamera.ReadParametersFromScene(SCENE_CONTAINER[sceneNumber]))
            {
                LOGE("Failed to parse camera parameters from scene "
                     << SCENE_CONTAINER[sceneNumber]);
                return false;
            }
        }

        if (!mScene.Load(SCENE_CONTAINER[sceneNumber]))
        {
            LOGE("Failed to parse scene JSON " << SCENE_CONTAINER[sceneNumber]);
            return false;
        }

        // emptyplane scene - fill it up
        if (sceneNumber == 1)
        {
            Scene::Containers::Ptr<Material::Matte> matte =
            std::dynamic_pointer_cast<Material::Matte>(
                mScene.CreateMaterial("sphereMatte", Types::Material::MATTE)
            );

            matte->SetColor(lkCommon::Utils::PixelFloat4(0.2f, 0.3f, 0.9f, 1.0f));

            Scene::Containers::Ptr<Geometry::Sphere> sphere;
            for (uint32_t i = 0; i < 1000; ++i)
            {
                std::string name = "sphere" + std::to_string(i);
                sphere = std::dynamic_pointer_cast<Geometry::Sphere>(
                    mScene.CreatePrimitive(name, Types::Primitive::SPHERE)
                );

                float radius = lkCommon::Math::Random::Xorshift() + 0.2f;
                sphere->SetPosition(lkCommon::Math::Vector4(
                    lkCommon::Math::Random::Xorshift() * 100.0f - 50.0f,
                    radius,
                    lkCommon::Math::Random::Xorshift() * 100.0f - 50.0f,
                    1.0f
                ));

                sphere->SetRadius(radius);
                sphere->SetMaterial(matte.get());
            }
        }

        mScene.BuildBVH();

        return true;
    }

    LKCOMMON_INLINE const Scene::Scene& GetScene() const
    {
        return mScene;
    }
};


int main(int argc, char* argv[])
{
    std::string rootdir;
#ifdef LKRAY_ROOT_DIR
    rootdir = LKRAY_ROOT_DIR;
#endif // LKRAY_ROOT_DIR
    lkCommon::Utils::Logger::SetRootPathToStrip(rootdir);

    if (!lkCommon::System::FS::SetCWD(
            lkCommon::System::FS::JoinPaths(
                lkCommon::System::FS::GetParentDir(lkCommon::System::FS::GetExecutablePath()),
                LKRAY_ROOT_REL_TO_BIN
                )
            )
        )
    {
        LOGE("Failed to set CWD");
        return -1;
    }

    int32_t scene = 0;
    int32_t threads = 0;

    lkCommon::Utils::ArgParser args;
    args.Add("s,scene", DEFAULT_SCENE, true,
             "Scene ID to load.");
    args.Add("t,threads", DEFAULT_THREAD_COUNT, true,
             "Amount of threads to render at. Defaults to logical CPU count in the system.");
    if (!args.Parse(argc, argv))
    {
        LOGE("Failed to parse args.");
        args.PrintUsage();
        return -1;
    }

    if (args.GetValue("threads", threads) && threads < 1)
    {
        LOGE("Invalid amount of rendering threads provided");
        return -1;
    }

    Renderer::Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT,
                                MAX_RAY_DEPTH_RENDERING, threads);
    renderer.SetExposure(EXPOSURE_DEFAULT);

    Scene::Camera camera(
        lkCommon::Math::Vector4(0.0f, 0.0f, -1.0f, 1.0f),
        lkCommon::Math::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
        -45.0f, 0.0f,
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

    args.GetValue('s', scene);
    if (!gWindow.LoadScene(static_cast<uint32_t>(scene)))
    {
        LOGE("Failed to load scene #" << scene);
        return -1;
    }

    lkCommon::Utils::Timer t;
    t.Start();
    float time = 0.0f;
    while (gWindow.IsOpened())
    {
        gWindow.Update(time);
        renderer.Draw(gWindow.GetScene(), camera);
        gWindow.DisplayImage(0, 0, renderer.GetOutput().GetWindowImage());

        time = static_cast<float>(t.Stop());
        t.Start();
        gFrameTime.Add(time);
    }

    gWindow.Close();
    return 0;
}
