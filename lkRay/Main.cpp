#include "PCH.hpp"

#include <lkCommon/System/Window.hpp>
#include <lkCommon/Utils/Timer.hpp>
#include <iostream>

lkCommon::Window gWindow;

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

int main()
{
    gWindow.Init();
    if (!gWindow.Open(300, 300, WINDOW_WIDTH, WINDOW_HEIGHT, "lkRay"))
    {
        return -1;
    }

    gWindow.SetTitle("lkRay");

    lkCommon::Timer t;
    t.Start();
    while (gWindow.IsOpen())
    {
        gWindow.Update(t.Stop());

        for (int i = 0; i < WINDOW_WIDTH; ++i)
        {
            for (int j = 0; j < WINDOW_HEIGHT; ++j)
            {
                //gWindow.SetPixel(i, j, (i << 16) | j);
            }
        }
    }

    return 0;
}
