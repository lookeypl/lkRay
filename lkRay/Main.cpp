#include "PCH.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/Timer.hpp>
#include <lkCommon/System/Window.hpp>
#include <lkCommon/System/FS.hpp>
#include <lkCommon/Math/RingAverage.hpp>

#include "Renderer/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Geometry/Sphere.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

using namespace lkRay;

Scene::Scene gScene;
Scene::Camera gCamera(
    lkCommon::Math::Vector4(0.0f, 0.0f, -3.0f, 1.0f),
    lkCommon::Math::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
    0.0f, 0.0f,
    60.0f, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)
);

lkCommon::Math::RingAverage<float, 20> gFrameTime;


class lkRayWindow: public lkCommon::System::Window
{
protected:
    void OnUpdate(float deltaTime) override
    {
        SetTitle("lkRay - " + std::to_string(gFrameTime.Get() * 1000.0f) + " ms");

        if (IsMouseKeyPressed(0))
        {
            float speed = 5.0f * deltaTime;
            if (IsKeyPressed(lkCommon::System::KeyCode::W)) gCamera.MoveForward(speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::S)) gCamera.MoveForward(-speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::D)) gCamera.MoveSideways(speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::A)) gCamera.MoveSideways(-speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::R)) gCamera.MoveWorldUp(speed);
            if (IsKeyPressed(lkCommon::System::KeyCode::F)) gCamera.MoveWorldUp(-speed);

            gCamera.Update();
        }
    }

    void OnMouseMove(int x, int y, int deltaX, int deltaY) override
    {
        if (IsMouseKeyPressed(0))
        {
            float shiftX = static_cast<float>(deltaX) * 0.005f;
            float shiftY = static_cast<float>(deltaY) * 0.005f;

            gCamera.RotateLeftRight(shiftX);
            gCamera.RotateUpDown(-shiftY);
        }
    }
};


int main()
{
    if (!lkCommon::System::FS::SetCWD(
            lkCommon::System::FS::JoinPaths(
                lkCommon::System::FS::GetParentDir(lkCommon::System::FS::GetExecutablePath()),
                "../../.."
                )
            )
        )
    {
        return -1;
    }

    lkRayWindow gWindow;
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

    Renderer::Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

    Geometry::Primitive::Ptr sphere = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Sphere>(
            lkCommon::Math::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
            1.0f
        )
    );
    if (!sphere)
    {
        return -1;
    }

    gScene.AddPrimitive(sphere);

    Scene::Light::Ptr light = std::make_shared<Scene::Light>(
        lkCommon::Math::Vector4(2.0f, 3.0f, -2.0f, 1.0f)
    );
    if (!light)
    {
        return -1;
    }

    gScene.AddLight(light);

    lkCommon::Utils::Timer t;
    t.Start();
    while (gWindow.IsOpen())
    {
        float time = static_cast<float>(t.Stop());
        t.Start();

        if (time > 0.001f)
            gFrameTime.Add(time);

        gWindow.Update(time);
        renderer.Draw(gScene, gCamera);
        gWindow.DisplayImage(0, 0, renderer.GetOutput());
    }

    return 0;
}
