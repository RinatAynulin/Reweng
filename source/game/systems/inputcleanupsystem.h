#pragma once

#include <ecs/system.h>

namespace Reweng
{
    class InputCleanupSystem : public CleanupSystem
    {
    public:
        InputCleanupSystem(std::shared_ptr<Context> pcontext);

        virtual void Clean() override;    
    };
}