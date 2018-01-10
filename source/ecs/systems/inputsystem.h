#pragma once

#include <ecs/context.h>
#include <ecs/system.h>

#include <thirdparty/glfw3/glfw3.h>

#include <array>

namespace Reweng
{
    namespace
    {
        constexpr int GetMaxKey()
        {
            return 1024;
        }
    }

    class InputSystem : public ISystem
    {
    public:
        InputSystem(Context::Ptr pcontext, GLFWwindow* wndHandle);

        virtual void Update(const double dt) override;

    private:
        void CreateInputComponent(const int key);

    private:
        GLFWwindow* WndHandle;
        std::array<int, GetMaxKey()> LastKeyStates;
    };
}