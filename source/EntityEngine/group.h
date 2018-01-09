#pragma once

#include <EntityEngine/eventmanager.h>

#include <memory>
#include <stack>
#include <tuple>
#include <vector>

namespace Reweng
{
    typedef unsigned int ComponentID;
    typedef unsigned int PositionInCache;
    typedef unsigned int EntityID;

    //class IEvent;
    class Entity;
    class Context;

    class Group
    {
    public:
        typedef std::shared_ptr<Group> Ptr;

    public:
        Group(const Context* context, std::vector<ComponentID> requirableComponentIDs);

        inline const std::vector<std::shared_ptr<Entity>>& GetEntities() const;
        inline const std::vector<ComponentID>              GetRequirableComponentIDs() const;

    private:
        void OnEntityAdded     (std::shared_ptr<IEvent> event);
        void OnEntityRemoved   (std::shared_ptr<IEvent> event);
        void OnComponentAdded  (std::shared_ptr<IEvent> event);
        void OnComponentRemoved(std::shared_ptr<IEvent> event);

        std::tuple<PositionInCache, bool> FindPositionWithID(const EntityID id) const;

    private:
        const Context* CurrentContext;

        std::vector<std::shared_ptr<Entity>> Entities;
        std::stack<PositionInCache> RemovedEntities;
        
        std::vector<ComponentID> RequirableComponentIDs;

        EventObserver Observer;
    };

    inline const std::vector<std::shared_ptr<Entity>>& Group::GetEntities() const
    {
        return Entities;
    }

    inline const std::vector<ComponentID> Group::GetRequirableComponentIDs() const
    {
        return RequirableComponentIDs;
    }
}