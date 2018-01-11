#include "game.h"

#include <ecs/system.h>
#include <ecs/component.h>
#include <game/components/inputcomponents.h>
#include <game/systems/inputsystem.h>
#include <game/systems/inputcleanupsystem.h>

#include <thirdparty/glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace Reweng
{
    struct PlayerControllable : public IComponent
    {};

    class PlayerController : public LogicSystem
    {
    public:
        std::shared_ptr<PlayerController> Ptr;

    public:
        PlayerController(Context::Ptr pContext)
            : LogicSystem(pContext)
        {
            playerGroup = pContext->GetGroup<PlayerControllable, MeshComponent>();
            inputGroup  = pContext->GetGroup<InputKey>();
        }

        virtual void Update(const double dt)
        {
            const auto& playerEntities = playerGroup->GetEntities();
            const auto& inputEntities = inputGroup->GetEntities();

            Entity::Ptr player = [playerEntities]() {
                for (const auto& player : playerEntities)
                {
                    if (player != nullptr)
                        return player;
                }
                GL_CHECK(false, "No suitable player has been found.");
            }();

            for (const auto& input : inputEntities)
            {
                if (input == nullptr)
                    continue;

                const InputKey* key = input->GetComponent<InputKey>();
                if (key->State != GLFW_PRESS)
                    continue;

                MeshComponent* mesh = player->GetComponent<MeshComponent>();

                switch (key->Key)
                {
                case GLFW_KEY_A:
                    mesh->Transform.RotationMatrix = glm::rotate(mesh->Transform.RotationMatrix, glm::radians(1.0f), glm::vec3{ 0,-1,0 });
                    std::cout << "A\n";
                    break;

                case GLFW_KEY_D:
                    mesh->Transform.RotationMatrix = glm::rotate(mesh->Transform.RotationMatrix, glm::radians(1.0f), glm::vec3{ 0,1,0 });
                    std::cout << "D\n";
                    break;
                }
            }
        }

    private:
        Group::Ptr playerGroup;
        Group::Ptr inputGroup;
    };

    class ResourceLoader : public InitializationSystem
    {
    public:

        ResourceLoader(Context::Ptr pContext, Renderer::Ptr prenderer)
            : InitializationSystem(pContext)
            , pRenderer(prenderer)
        {            
        }

        virtual void Initialize()
        {
            LoadShaders();
            LoadTextures();
        }

    private:
        void LoadShaders()
        {
            GLShaderCreateInfo createInfo = {};
            createInfo.VertexShaderFSLocation   = "resources/shaders/vertex.vert";
            createInfo.FragmentShaderFSLocation = "resources/shaders/fragment.frag";

            pRenderer->ShaderManager.UploadShader(createInfo, "general");
        }

        void LoadTextures() 
        {
            pRenderer->TextureManager.Upload2DTexture("resources/textures/container.jpg", "container");
        }

    private:
        Renderer::Ptr pRenderer;
    };

    class LevelConstructor : public InitializationSystem
    {
    public:
        LevelConstructor(Context::Ptr pcontext, Renderer::Ptr prenderer)
            : InitializationSystem(pcontext)
            , pRenderer(prenderer)
        {
        }

        virtual void Initialize() override
        {
            Entity::Ptr player = pContext->CreateEntity();            

            MeshComponent mesh = pRenderer->MeshManager.GetMesh("cube");
            mesh.Material.Shader = pRenderer->ShaderManager.GetShader("general");
            mesh.Material.Texture = pRenderer->TextureManager.GetTexture("container");
            TransformComponent transform = {};
            transform.Position = { 0,0,0 };
            transform.Scale = { 1,1,1 };
            mesh.Transform = transform;
            player->AddComponent<MeshComponent>(mesh);

            CameraComponent camera = {};
            camera.View = glm::translate(glm::mat4(), { 0,0,-2 });
            camera.Projection = glm::perspective(45.0f, (float)pRenderer->GetWindowExtent().Width / (float)pRenderer->GetWindowExtent().Height, 0.1f, 100.0f);
            player->AddComponent<CameraComponent>(camera);
            pRenderer->SetCamera(camera);

            player->AddComponent<PlayerControllable>();
        }

    private:
        Renderer::Ptr pRenderer;
    };


    class MeshRenderer : public LogicSystem
    {
    public:
        MeshRenderer(Context::Ptr pcontext, Renderer::Ptr prenderer)
            : LogicSystem(pcontext)
            , pRenderer(prenderer)
        {
            MeshGroup = pContext->GetGroup<MeshComponent>();
        }

        virtual void Update(const double dt)
        {
            const auto& entities = MeshGroup->GetEntities();
            for (const auto& entity : entities)
            {
                if (entity == nullptr)
                    return;

                MeshComponent* mesh = entity->GetComponent<MeshComponent>();
                pRenderer->Submit(*mesh); //todo ref -> ptr in renderer
            }

            pRenderer->Flush();
        }

    private:
        Renderer::Ptr pRenderer;
        Group::Ptr MeshGroup;
    };

    Game::Game()
        : pWindow(new WindowHolder())
        , pContext(new Context())
        , pRenderer(new Renderer())
    {
        pWindow->ConstructWindow({ 800,600 }, "test");
        pRenderer->Init(pWindow);
        
        InitializationSystem::Ptr resourceLoader(new ResourceLoader(pContext, pRenderer));
        InitializationSystem::Ptr levelConstructor(new LevelConstructor(pContext, pRenderer));

        LogicSystem::Ptr inputSystem(new InputSystem(pContext, pWindow->GetHandle()));
        LogicSystem::Ptr playerController(new PlayerController(pContext));
        LogicSystem::Ptr meshRenderer(new MeshRenderer(pContext, pRenderer));

        CleanupSystem::Ptr inputCleanupSystem(new InputCleanupSystem(pContext));

        pContext->AddInitializationSystem(resourceLoader);
        pContext->AddInitializationSystem(levelConstructor);
        pContext->AddLogicSystem(inputSystem);
        pContext->AddLogicSystem(playerController);
        pContext->AddLogicSystem(meshRenderer);
        pContext->AddCleanupSystem(inputCleanupSystem);
    }
}