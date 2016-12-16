#pragma once

namespace lkRay {
namespace Common {

class Window
{
    HINSTANCE mInstance;
    HWND mHWND;
    HDC mHDC;
    int mWidth;
    int mHeight;
    bool mOpened;
    std::wstring mClassName;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    Window();
    ~Window();

    bool Init();
    bool Open(int x, int y, int width, int height, const std::string& title);
    void ProcessMessages();
    void Close();

    void SetPixel(int x, int y, COLORREF color);

    bool IsOpen() const;

protected: // callbacks
    virtual void OnClose();
};

} // namespace Common
} // namespace lkRay
