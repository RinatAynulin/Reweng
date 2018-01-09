#pragma once

namespace Reweng
{
    struct IComponent
    {
        IComponent()
            :IsActive(true)
        {
        }

        bool IsActive;
    };

    typedef unsigned int ComponentID;
}