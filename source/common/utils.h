#pragma once

#include <string>

namespace Reweng::Utils
{
    std::wstring ConvertStringToWstring(const std::string& str)
    {
        return std::wstring(str.begin(), str.end());
    }

    std::string ConvertWStringToString(const std::wstring& str)
    {
        return std::string(str.begin(), str.end());
    }
}