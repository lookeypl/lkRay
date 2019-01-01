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
#include "Material/Matte.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
const uint32_t MAX_RAY_DEPTH_MOVEMENT = 1;
const uint32_t MAX_RAY_DEPTH_RENDERING = 4;
const float EXPOSURE_DEFAULT = 0.7f;

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
                "../../.."
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


    Material::Matte blue;
    blue.SetColor(lkCommon::Utils::PixelFloat4(0.2f, 0.5f, 0.9f, 1.0f));

    Material::Matte white;
    white.SetColor(lkCommon::Utils::PixelFloat4(0.9f, 0.9f, 0.9f, 1.0f));

    Material::Matte red;
    red.SetColor(lkCommon::Utils::PixelFloat4(0.9f, 0.4f, 0.2f, 1.0f));

    Material::Matte reflective;
    reflective.SetColor(lkCommon::Utils::PixelFloat4(0.6f, 0.3f, 0.9f, 1.0f));

    Geometry::Primitive::Ptr sphere = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Sphere>(
            lkCommon::Math::Vector4(-0.7f,-0.5f,-0.7f, 1.0f),
            1.0f
        )
    );
    sphere->SetMaterial(&reflective);
    scene.AddPrimitive(sphere);

    Geometry::Primitive::Ptr sphere2 = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Sphere>(
            lkCommon::Math::Vector4(2.5f, 0.0f, 2.5f, 1.0f),
            1.5f
        )
    );
    sphere2->SetMaterial(&blue);
    scene.AddPrimitive(sphere2);

    Geometry::Primitive::Ptr sphere3 = std::dynamic_pointer_cast<Geometry::Primitive>(
        std::make_shared<Geometry::Sphere>(
            lkCommon::Math::Vector4(-1.5f, 0.5f, 2.5f, 1.0f),
            2.0f
        )
    );
    sphere3->SetMaterial(&red);
    scene.AddPrimitive(sphere3);


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
    mesh->SetMaterial(&white);
    scene.AddPrimitive(mesh);

    Scene::Light::Ptr light = std::make_shared<Scene::Light>(
        lkCommon::Math::Vector4(2.5f, 4.0f,-2.5f, 1.0f),
        lkCommon::Utils::PixelFloat4(0.5f),
        0.2f
    );
    scene.AddLight(light);

    Scene::Light::Ptr light2 = std::make_shared<Scene::Light>(
        lkCommon::Math::Vector4(-2.5f, 4.0f,-2.5f, 1.0f),
        lkCommon::Utils::PixelFloat4(0.5f),
        0.2f
    );
    scene.AddLight(light2);

    scene.SetAmbient(lkCommon::Utils::PixelFloat4({0.1f, 0.1f, 0.1f, 1.0f}));


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
