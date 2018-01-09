#pragma once

#include "component.h"

#include <set>
#include <vector>

namespace Reweng
{
    class ComponentIdFromType
    {
    public:
        template<class ComponentType>
        constexpr static ComponentID Get()
        {
            static_assert((std::is_base_of<IComponent, ComponentType>::value == true) && (std::is_same<IComponent, ComponentType>::value == false),
                "Unable to get a component type id from the Class that is not derived from IComponent.");

            static ComponentID typeID = typeCount++;

            return typeID;
        }

    private:
        static ComponentID typeCount;
    };

    class ComponentIdsFromPack
    {
    public:
        template<class ...Args>
        constexpr static std::vector<ComponentID> Get()
        {
            return { ComponentIdFromType::Get<Args>()... };
        }

        template<class ...Args>
        constexpr static std::set<ComponentID> GetSet()
        {
            return { ComponentIdFromType::Get<Args>()... };
        }
    };
}