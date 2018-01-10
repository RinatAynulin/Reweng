#pragma once

#include <ecs/component.h>

namespace Reweng
{
    struct InputKey : public IComponent
    {
        int Key;
        int State;
    };
}