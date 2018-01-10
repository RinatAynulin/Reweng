#pragma once

#include <common/error.h>
#include <ecs/componentpool.h>
#include <ecs/componentutils.h>
#include <ecs/entity.h>
#include <ecs/group.h>
#include <ecs/system.h>
#include <events/eventmanager.h>

#include <memory>
#include <stack>
#include <type_traits>
#include <vector>

namespace Reweng
{
    class Context
    {
    public:
        typedef std::shared_ptr<Context> Ptr;
    
    public:
        Context();

        void                                  Loop();

        inline void                           RemoveEntity(const EntityID id);
        inline EventManager::Ptr              GetEventManager() const;        
        inline std::vector<Entity::Ptr>       GetEntities() const;
        inline Entity::Ptr                    GetEntity(const EntityID id) const;
        Entity::Ptr                           CreateEntity();        

        inline void                           AddInitializationSystem(InitializationSystem::Ptr system);
        inline void                           AddLogicSystem(LogicSystem::Ptr system);
        inline void                           AddCleanupSystem(CleanupSystem::Ptr system);

        template<class ...Components>
        Group::Ptr                            GetGroup();
        

        //void RemoveSystem(const SystemID id);
    private:
        EventManager::Ptr                      pEventManager;

        EntityID                               entityCount;
        std::vector<Entity::Ptr>               Entities;
        std::vector<Group::Ptr>                Groups;
        std::stack<EntityID>                   ReusableEntities;

        std::vector<InitializationSystem::Ptr> InitializationSystems;
        std::vector<LogicSystem::Ptr>          LogicSystems;        
        std::vector<CleanupSystem::Ptr>        CleanupSystems;

        ComponentPools::Ptr                    pComponentPools;
    };

    inline void Context::RemoveEntity(const EntityID id)
    {
        GL_CHECK(id < Entities.size(), "Incorrect entity id.");
        Entities[id]->Disable();
        ReusableEntities.push(id);
    }

    inline EventManager::Ptr Context::GetEventManager() const
    {
        return pEventManager;
    }

    inline void Context::AddInitializationSystem(InitializationSystem::Ptr system)
    {
        InitializationSystems.push_back(system);
    }

    inline void Context::AddLogicSystem(LogicSystem::Ptr system)
    {
        LogicSystems.push_back(system);
    }

    inline void Context::AddCleanupSystem(CleanupSystem::Ptr system)
    {
        CleanupSystems.push_back(system);
    }

    inline std::vector<Entity::Ptr> Context::GetEntities() const
    {
        return Entities;
    }

    inline Entity::Ptr Context::GetEntity(const EntityID id) const
    {
        GL_CHECK(id < Entities.size(), "Incorrect entity id.");
        return Entities[id];
    }

    template<class ...ComponentsToObserve>
    Group::Ptr Context::GetGroup()
    {
        const std::set<ComponentID> idsToObserve = ComponentIdsFromPack::GetSet<ComponentsToObserve...>();
        for (const auto& group : Groups)
        {
            std::vector<ComponentID> requiredByGroupIDs = group->GetRequirableComponentIDs();
            std::set<ComponentID> groupIdsSet(requiredByGroupIDs.begin(), requiredByGroupIDs.end());

            if (idsToObserve == groupIdsSet)
                return group;
        }

        Group::Ptr newGroup{ new Group{this, ComponentIdsFromPack::Get<ComponentsToObserve...>()} };
        Groups.push_back(newGroup);

        return newGroup;
    }
}