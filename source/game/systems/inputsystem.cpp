#include "inputsystem.h"

#include <game/components/inputcomponents.h>

namespace Reweng
{
    InputSystem::InputSystem(Context::Ptr pcontext, GLFWwindow* wndHandle)
        : LogicSystem(pcontext)
        , WndHandle(wndHandle)
    {
        glfwSetInputMode(WndHandle, GLFW_STICKY_KEYS, 1);
    }

    void InputSystem::Update(const double dt)
    {
        glfwPollEvents();
        CreateInputComponent(GLFW_KEY_A);
        CreateInputComponent(GLFW_KEY_S);
        CreateInputComponent(GLFW_KEY_D);
        CreateInputComponent(GLFW_KEY_W);
    }

    void InputSystem::CreateInputComponent(const int key)
    {
        const int lastState = LastKeyStates[key];
        const int newState  = glfwGetKey(WndHandle, key);
        LastKeyStates[key]  = newState;

        if (((lastState == GLFW_PRESS) && (newState == GLFW_RELEASE)) || (newState == GLFW_PRESS))
        {
            Entity::Ptr entity = pContext->CreateEntity();
            InputKey inputKey = {};
            inputKey.Key = key;
            inputKey.State = newState;
            entity->AddComponent<InputKey>(inputKey);
        }
    }
}