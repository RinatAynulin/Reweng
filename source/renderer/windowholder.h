#pragma once

#include <common/error.h>

#include <thirdparty/glad/glad.h>
#include <thirdparty/glfw3/glfw3.h>

namespace Reweng
{
    struct Extent2D
    {
        uint16_t Width, Height;
    };

    class WindowHolder final
    {
    public:
        inline WindowHolder()
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        }

        inline void ConstructWindow(Extent2D extent, const char* title)
        {
            Handle = glfwCreateWindow(extent.Width, extent.Height, title, nullptr, nullptr);
            GL_CHECK(Handle != nullptr, "Unable to create a window.");
            Extent = extent;
        }
        
        inline GLFWwindow* GetHandle()
        {
            return Handle;
        }

        inline Extent2D GetExtent()
        {
            return Extent;
        }

    private:
        GLFWwindow* Handle = nullptr;
        Extent2D Extent = {};
    };
}