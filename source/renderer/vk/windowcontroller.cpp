#include "windowcontroller.h"

#include "common\error.h"

#include <utility>

namespace Reweng
{
#ifdef _WIN32
    WindowController::WindowController(std::wstring&& applicationName)
        : Hwnd(0)
        , ApplicationName(std::forward<decltype(applicationName)>(applicationName))
    {
        HInstance = GetModuleHandle(NULL);
        if (HInstance == 0)
            throw Common::Error("GetModuleHandle returned 0.", Common::Critical);
    }

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void WindowController::ConstructWindow(const unsigned int width, const unsigned int height)
    {
        WNDCLASSEX wndclassex = {};
        wndclassex.cbSize = sizeof(WNDCLASSEX);
        wndclassex.lpfnWndProc = WindowProc;
        wndclassex.hInstance = HInstance;
        wndclassex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wndclassex.lpszClassName = "Rewclass";

        if (RegisterClassEx(&wndclassex) == 0)
            throw Common::Error("RegisterClassEx returned 0.", Common::Critical).AddWinError(GetLastError());

        Hwnd = CreateWindowEx(0, wndclassex.lpszClassName, "Rewland window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100,
                              width, height, NULL, NULL, HInstance, NULL);
        if (Hwnd == 0)
            throw Common::Error("CreateWindowEx returned 0.", Common::Critical).AddWinError(GetLastError());

        ShowWindow(Hwnd, SW_SHOW);
    }
#endif
}