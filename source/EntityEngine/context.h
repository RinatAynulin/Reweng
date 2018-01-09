#pragma once

#include <common/error.h>
#include <EntityEngine/componentpool.h>
#include <EntityEngine/componentutils.h>
#include <EntityEngine/entity.h>
#include <EntityEngine/eventmanager.h>
#include <EntityEngine/group.h>
#include <EntityEngine/system.h>

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

        void                            Loop();
        inline void                     RemoveEntity(const EntityID id);
        inline EventManager::Ptr        GetEventManager() const;        
        inline std::vector<Entity::Ptr> GetEntities() const;
        inline Entity::Ptr              GetEntity(const EntityID id) const;
        Entity::Ptr                     CreateEntity();
        void                            Step(const double dt);

        template<class Type>
        inline void                     AddSystem(ISystem::Ptr system);

        template<class ...Components>
        Group::Ptr                      GetGroup();
        

        //void RemoveSystem(const SystemID id);
    private:
        EventManager::Ptr         pEventManager;

        EntityID                  entityCount;
        std::vector<Entity::Ptr>  Entities;
        std::vector<Group::Ptr>   Groups;
        std::stack<EntityID>      ReusableEntities;

        std::vector<ISystem::Ptr> LogicSystems;
        std::vector<ISystem::Ptr> InitializationSystems;
        std::vector<ISystem::Ptr> CleanupSystems;

        ComponentPools::Ptr       pComponentPools;
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

    template<class Type>
    inline void Context::AddSystem(ISystem::Ptr system)
    {
        GL_CHECK(system != nullptr, "Unable to add a not constructed system.");

        static_assert(std::is_base_of<SystemType, Type>::value && (std::is_same<SystemType, Type>::value == false)
            , "Incorrect system type: system type have to be derived from SystemType class.");

        if constexpr(std::is_same<Type, InitializationSystemType>::value)
            InitializationSystems.push_back(system);
        else if constexpr(std::is_same<Type, LogicSystemType>::value)
            LogicSystems.push_back(system);
        else if constexpr(std::is_same<Type, CleanupSystemType>::value)
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