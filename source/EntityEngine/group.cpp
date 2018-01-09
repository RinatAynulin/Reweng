#include "group.h"

#include <common/error.h>
#include <EntityEngine/context.h>
#include <EntityEngine/entity.h>
#include <EntityEngine/entityevents.h>

namespace Reweng
{
    Group::Group(const Context* context, std::vector<ComponentID> requirableComponentIDs)
        : CurrentContext(context)
        , RequirableComponentIDs(requirableComponentIDs)
        , Observer(CurrentContext->GetEventManager())
    {
        Observer.OnEvent<EntityAdded>([this](IEvent::Ptr event)
        {
            OnEntityAdded(event);
        });

        Observer.OnEvent<EntityRemoved>([this](IEvent::Ptr event)
        {
            OnEntityRemoved(event);
        });

        Observer.OnEvent<ComponentAdded>([this](IEvent::Ptr event)
        {
            OnComponentAdded(event);
        });

        Observer.OnEvent<ComponentRemoved>([this](IEvent::Ptr event)
        {
            OnComponentRemoved(event);
        });

        const auto entities = context->GetEntities();
        for (const auto& entity : entities)
        {
            if (entity->HasComponents(RequirableComponentIDs))
                Entities.push_back(entity);
        }
    }

    void Group::OnEntityAdded(std::shared_ptr<IEvent> event)
    {
        const EntityAdded* castedEvent = event->As<EntityAdded*>();
        const Entity::Ptr  addedEntity = castedEvent->AddedEntity;
        
        if (addedEntity->HasComponents(RequirableComponentIDs) == false)
            return;

        if (RemovedEntities.empty() == false)
        {
            const PositionInCache position = RemovedEntities.top();
            GL_CHECK(Entities[position] == nullptr, "Group cached ids has an activated entity.");
            Entities[position] = addedEntity;

            return;
        }

        Entities.push_back(addedEntity);
    }

    void Group::OnEntityRemoved(std::shared_ptr<IEvent> event)
    {
        const EntityRemoved* castedEvent   = event->As<EntityRemoved*>();
        const Entity::Ptr    removedEntity = castedEvent->RemovedEntity;

        if (removedEntity->HasComponents(RequirableComponentIDs) == false)
            return;
        
        auto[positionInCache, isFound] = FindPositionWithID(removedEntity->GetID());
        GL_CHECK(isFound, "Entity sutisfies with required component ids but it hasn't been in group when EntityRemoved trigerred.");
        GL_CHECK(Entities[positionInCache] == nullptr, "chunga changa.");

        Entities[positionInCache] = nullptr;
        RemovedEntities.push(positionInCache);
    }
    
    void Group::OnComponentAdded(std::shared_ptr<IEvent> event)
    {
        const ComponentAdded* castedEvent = event->As<ComponentAdded*>();
        const EntityID ownerID = castedEvent->OwnerID;

        auto[positionInCache, isFound] = FindPositionWithID(ownerID);
        if (isFound)
            return;

        Entity::Ptr eventSender = CurrentContext->GetEntity(ownerID);
        if (eventSender->HasComponents(RequirableComponentIDs))
        {
            if (RemovedEntities.empty() == false)
            {
                const PositionInCache position = RemovedEntities.top();
                GL_CHECK(Entities[position] == nullptr, "Removed entity in group is not nullptr.");
                Entities[position] = eventSender;
                RemovedEntities.pop();

                return;
            }

            Entities.push_back(eventSender);
        }
    }

    void Group::OnComponentRemoved(std::shared_ptr<IEvent> event)
    {
        const ComponentRemoved* castedEvent = event->As<ComponentRemoved*>();
        const EntityID ownerID = castedEvent->OwnerID;
        
        auto[positionInCache, isFound] = FindPositionWithID(ownerID);
        if (isFound)
        {
            Entity::Ptr& entity = Entities[positionInCache];
            if (entity->HasComponents(RequirableComponentIDs) == false)
                Entities[positionInCache] = nullptr;

            RemovedEntities.push(positionInCache);
        }
    }

    std::tuple<PositionInCache, bool> Group::FindPositionWithID(const EntityID id) const
    {
        for (int i = 0; i < Entities.size(); ++i)
        {
            if (Entities[i] == nullptr)
                continue;

            if (id == Entities[i]->GetID())
                return { i, true };
        }

        return { -1, false };
    }
}