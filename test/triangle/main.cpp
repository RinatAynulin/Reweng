#include <ecs/context.h>
#include <ecs/component.h>
#include <game/systems/inputsystem.h>
#include <game/components/inputcomponents.h>
#include <game/systems/inputcleanupsystem.h>

#include <exception>
#include <iostream>
#include <string>

using namespace Reweng;

class System : public LogicSystem
{
public:

    System(Context::Ptr context)
        : LogicSystem(context)
    {
       Group = pContext->GetGroup<InputKey>();
    }

    virtual void Update(const double dt) override
    {
        Group::Ptr group = pContext->GetGroup<InputKey>();
        for (const auto& entity : group->GetEntities())
        {
            if (entity == nullptr)
                continue;

            InputKey* ik = entity->GetComponent<InputKey>();
            std::cout << "key:" << ik->Key << " state:" << ik->State << std::endl;
        }

    }

    Group::Ptr Group;
};

int main()
{
    try
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWwindow* Window = glfwCreateWindow(800, 600, "oblya", nullptr, nullptr);
        GL_CHECK(Window != nullptr, "Unable to create a window.");
        glfwMakeContextCurrent(Window);

        Context::Ptr context(new Context);
        LogicSystem::Ptr pSystem(new System(context));
        LogicSystem::Ptr pInput(new InputSystem(context, Window));
        CleanupSystem::Ptr pInputCleanup(new InputCleanupSystem(context));

        context->AddLogicSystem(pInput);
        context->AddLogicSystem(pSystem);
        context->AddCleanupSystem(pInputCleanup);

        context->Loop();

        volatile bool flag = true;
        while (flag)
        {
            
        }

        
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }
}