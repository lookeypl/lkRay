#include "PCH.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/System/Window.hpp>
#include <lkCommon/Utils/Timer.hpp>
#include <lkCommon/Utils/Image.hpp>

lkCommon::Window gWindow;

uint32_t WINDOW_WIDTH = 1280;
uint32_t WINDOW_HEIGHT = 720;

int main()
{
    gWindow.Init();
    if (!gWindow.Open(300, 300, WINDOW_WIDTH, WINDOW_HEIGHT, "lkRay"))
    {
        LOGE("Failed to open window");
        return -1;
    }

    gWindow.SetTitle("lkRay");

    lkCommon::Image result;
    if (!result.Resize(WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        LOGE("Failed to resize result Image");
        return -1;
    }

    lkCommon::Timer t;
    t.Start();
    while (gWindow.IsOpen())
    {
        gWindow.Update(static_cast<float>(t.Stop()));

        for (uint32_t x = 0; x < WINDOW_WIDTH; ++x)
        {
            for (uint32_t y = 0; y < WINDOW_HEIGHT; ++y)
            {
                float r = static_cast<float>(x) / static_cast<float>(WINDOW_WIDTH) * 255.0f;
                float b = static_cast<float>(y) / static_cast<float>(WINDOW_HEIGHT) * 255.0f;
                result.SetPixel(x, y, static_cast<uint8_t>(r), static_cast<uint8_t>(0), static_cast<uint8_t>(b));
            }
        }

        gWindow.DisplayImage(0, 0, result);
    }

    return 0;
}
