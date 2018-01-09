#pragma once

#include <EntityEngine/component.h>

namespace Reweng
{
    struct InputKey : public IComponent
    {
        int Key;
        int State;
    };
}