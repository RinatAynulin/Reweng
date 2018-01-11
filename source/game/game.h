#pragma once

#include <renderer/renderer.h>
#include <renderer/windowholder.h>
#include <ecs/context.h>

namespace Reweng
{
    class Game
    {
    public:
        Game();
        inline void Start();
    private:
        WindowHolder::Ptr pWindow;
        Context::Ptr      pContext;
        Renderer::Ptr     pRenderer;
    };

    inline void Game::Start()
    {
        pContext->Loop();
    }
}