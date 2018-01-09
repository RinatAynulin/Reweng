#pragma once

#include <EntityEngine/event.h>

#include <memory>

namespace Reweng
{
    class Entity;
    typedef unsigned int EntityID;

    struct EntityAdded : public IEvent
    {
        EntityAdded(std::shared_ptr<Entity> entity)
            : AddedEntity(entity)
        {
        }

        std::shared_ptr<Entity> AddedEntity;
    };

    struct EntityRemoved : public IEvent
    {
        EntityRemoved(std::shared_ptr<Entity> entity)
            : RemovedEntity(entity)
        {
        }
        std::shared_ptr<Entity> RemovedEntity;
    };

    struct ComponentRemoved : public IEvent
    {
        ComponentRemoved(EntityID id, ComponentID removedComponentID)
            : OwnerID(id)
            , RemovedComponentID(removedComponentID)
        {
        }
        EntityID OwnerID;
        ComponentID RemovedComponentID;
    };

    struct ComponentAdded : public IEvent
    {
        ComponentAdded(EntityID id, ComponentID addedComponentID)
            : OwnerID(id)
            , AddedComponentID(addedComponentID)
        {
        }
        EntityID OwnerID;
        ComponentID AddedComponentID;
    };
}