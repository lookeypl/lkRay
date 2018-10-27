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
#include "Geometry/Plane.hpp"
#include "Geometry/Mesh.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

using namespace lkRay;

Renderer::Renderer gRenderer(WINDOW_WIDTH, WINDOW_HEIGHT);
Scene::Scene gScene;
Scene::Camera gCamera(
    lkCommon::Math::Vector4(0.0f, 1.0f, -7.0f, 1.0f),
    lkCommon::Math::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
    0.0f, 0.0f,
    75.0f, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)
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

    void OnResize(int width, int height) override
    {
        gRenderer.ResizeOutput(width, height);
        gCamera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
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


    Scene::Material blue;
    blue.SetColor(lkCommon::Utils::PixelFloat4({0.2f, 0.5f, 0.9f, 1.0f}));

    Scene::Material yellow;
    yellow.SetColor(lkCommon::Utils::PixelFloat4({0.9f, 0.9f, 0.2f, 1.0f}));

    Scene::Material red;
    red.SetColor(lkCommon::Utils::PixelFloat4({0.9f, 0.4f, 0.2f, 1.0f}));


    Geometry::Primitive::Ptr sphere = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Sphere>(
            lkCommon::Math::Vector4(0.7f,-0.5f, 0.7f, 1.0f),
            1.0f
        )
    );
    gScene.AddPrimitive(sphere);

    Geometry::Primitive::Ptr sphere2 = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Sphere>(
            lkCommon::Math::Vector4(2.5f, 0.0f, 2.5f, 1.0f),
            1.5f
        )
    );
    sphere2->SetMaterial(blue);
    gScene.AddPrimitive(sphere2);

    Geometry::Primitive::Ptr sphere3 = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Sphere>(
            lkCommon::Math::Vector4(-1.5f, 0.5f, 2.5f, 1.0f),
            2.0f
        )
    );
    sphere3->SetMaterial(red);
    gScene.AddPrimitive(sphere3);

    std::vector<lkCommon::Math::Vector4> points;
    points.emplace_back( 5.0f,-1.5f, 5.0f, 1.0f); // 0 + - +
    points.emplace_back( 5.0f, 4.5f, 5.0f, 1.0f); // 1 + + +
    points.emplace_back(-5.0f,-1.5f, 5.0f, 1.0f); // 2 - - +
    points.emplace_back(-5.0f, 4.5f, 5.0f, 1.0f); // 3 - + +

    points.emplace_back( 5.0f,-1.5f,-5.0f, 1.0f); // 4 + - -
    points.emplace_back( 5.0f, 4.5f,-5.0f, 1.0f); // 5 + + -
    points.emplace_back(-5.0f,-1.5f,-5.0f, 1.0f); // 6 - - -
    points.emplace_back(-5.0f, 4.5f,-5.0f, 1.0f); // 7 - + -

    std::vector<Geometry::Triangle> tris;
    // back wall
    tris.emplace_back(0, 1, 2);
    tris.emplace_back(1, 3, 2);
    // front wall
    tris.emplace_back(4, 6, 5);
    tris.emplace_back(5, 6, 7);
    // right wall
    tris.emplace_back(0, 4, 1);
    tris.emplace_back(1, 4, 5);
    // left wall
    tris.emplace_back(2, 3, 6);
    tris.emplace_back(3, 7, 6);
    // floor
    tris.emplace_back(4, 2, 6);
    tris.emplace_back(4, 0, 2);
    // ceiling
    tris.emplace_back(3, 1, 5);
    tris.emplace_back(3, 5, 7);

    Geometry::Primitive::Ptr mesh = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Mesh>(
            lkCommon::Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f),
            points, tris
        )
    );
    mesh->SetMaterial(yellow);
    gScene.AddPrimitive(mesh);

    Scene::Light::Ptr light = std::make_shared<Scene::Light>(
        lkCommon::Math::Vector4(2.5f, 4.0f,-2.5f, 1.0f),
        static_cast<lkCommon::Utils::PixelFloat4>(lkCommon::Utils::PixelUint4({0x85, 0xBE, 0x3C, 0xFF})),
        0.1f
    );
    gScene.AddLight(light);

    Scene::Light::Ptr light2 = std::make_shared<Scene::Light>(
        lkCommon::Math::Vector4(-2.5f, 4.0f,-2.5f, 1.0f),
        lkCommon::Utils::PixelFloat4({0.3f, 0.7f, 1.0f, 1.0f}),
        0.1f
    );
    gScene.AddLight(light2);


    lkCommon::Utils::Timer t;
    t.Start();
    while (gWindow.IsOpen())
    {
        float time = static_cast<float>(t.Stop());
        t.Start();

        if (time > 0.001f)
            gFrameTime.Add(time);

        gWindow.Update(time);
        gRenderer.Draw(gScene, gCamera);
        gWindow.DisplayImage(0, 0, gRenderer.GetOutput());
    }

    return 0;
}
