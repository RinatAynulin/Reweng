#pragma once

#include <ecs/system.h>

namespace Reweng
{
    class InputCleanupSystem : public ISystem
    {
    public:
        InputCleanupSystem(std::shared_ptr<Context> pcontext);

        virtual void Update(const double dt) override;    
    };
}