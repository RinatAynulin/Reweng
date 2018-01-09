#pragma once

#ifdef _WIN32
#include <windows.h>
#include <WinUser.h>
#include <wingdi.h>
#endif
#include <string>

namespace Reweng
{
    class [[deprecated]] WindowController final
    {
    public:
        WindowController(std::wstring&& applicationName);

        void ConstructWindow(const unsigned int width, const unsigned int height);

#ifdef _WIN32
    public:
        inline HINSTANCE GetHInstance() const
        {
            return HInstance;
        }
        inline HWND GetHwnd() const
        {
            return Hwnd;
        }
    private:
        HINSTANCE HInstance;
        HWND Hwnd;
        const std::wstring ApplicationName;
#else
        static_assert(!"WindowController is not supported by you OS");
#endif
    };
}