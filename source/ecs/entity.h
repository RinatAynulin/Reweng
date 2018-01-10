#pragma once

#include <common/error.h>
#include <ecs/component.h>
#include <ecs/componentpool.h>
#include <ecs/componentutils.h>
#include <ecs/entityevents.h>
#include <events/eventmanager.h>

#include <map>
#include <memory>
#include <utility>

namespace Reweng
{
    typedef unsigned int EntityID;

    class Entity
    {
    public:
        typedef std::shared_ptr<Entity> Ptr;
    
    public:
        inline explicit Entity(EventManager::Ptr peventManager, ComponentPools::Ptr pools, const EntityID id);
        inline explicit Entity(Entity&& entity);
        
        inline void     operator=(Entity&& entity);
        
        inline const    EntityID GetID() const;
        inline bool     IsActivated() const;
        inline void     Enable();
        inline void     Disable();

        template<class ComponentType>
        inline bool HasComponent() const;

        template<class ...ComponentTypes>
        bool HasComponents() const;

        bool HasComponents(const std::vector<ComponentID>& ids) const;

        template<class ComponentType, class ...Args>
        inline void AddComponent(Args&&... args);

        template<class ComponentType>
        inline ComponentType* GetComponent();

        template<class ComponentType>
        inline void RemoveComponent();

        inline void RemoveAllComponents();

    private:
        template<class ComponentType>
        PositionInPool GetPositionInPool();

    private:
        EventManager::Ptr pEventManager;
        bool IsActive;
        unsigned int ID;
        ComponentPools::Ptr pComponentPools;
        std::map<ComponentID, PositionInPool> Components;
    };


    inline Entity::Entity(EventManager::Ptr peventManager, ComponentPools::Ptr pools, const EntityID id)
        : pEventManager(peventManager)
        , IsActive(true)
        , ID(id)
        , pComponentPools(pools)
    {
    }

    inline  Entity::Entity(Entity&& entity)
        : pEventManager(entity.pEventManager)
        , IsActive(true)
        , ID(entity.ID)
        , pComponentPools(entity.pComponentPools)
    {
        entity.pEventManager = nullptr;
        entity.IsActive = false;
        ID = -1;
        entity.pComponentPools = nullptr;
    }

    inline void  Entity::operator=(Entity&& entity)
    {
        pEventManager = entity.pEventManager;
        IsActive = true;
        ID = entity.ID;
        pComponentPools = entity.pComponentPools;

        entity.pEventManager = nullptr;
        entity.IsActive = false;
        entity.ID = -1;
        entity.pComponentPools = nullptr;
    }

    inline const EntityID  Entity::GetID() const
    {
        return ID;
    }

    inline bool  Entity::IsActivated() const
    {
        return IsActive;
    }

    inline void  Entity::Enable()
    {
        IsActive = true;
    }

    inline void  Entity::Disable()
    {
        RemoveAllComponents();
        IsActive = false;
    }

    template<class ComponentType>
    inline bool Entity::HasComponent() const
    {
        GL_CHECK(IsActive, "Attempt to use a disabled entity.");
        const bool isComponentAdded = Components.find(ComponentIdFromType::Get<ComponentType>()) != Components.end();
        
        return isComponentAdded;
    }

    template<class ...ComponentTypes>
    bool Entity::HasComponents() const
    {
        GL_CHECK(IsActive, "Attempt to use a disabled entity.");

        const std::vector<ComponentID> componentIDs = ComponentIdsFromPack::Get<ComponentTypes...>();
        return HasComponents(componentIDs);
    }

    template<class ComponentType, class ...Args>
    inline void Entity::AddComponent(Args&&... args)
    {
        GL_CHECK(IsActive, "Attempt to use a disabled entity.");

        ComponentID componentID = ComponentIdFromType::Get<ComponentType>();
        GL_CHECK(Components.find(componentID) == Components.end(), "Unable to add a new component to the entity: component already exist.");

        const PositionInPool position = pComponentPools->AddComponent<ComponentType>(std::forward<Args>(args)...);
        Components[componentID] = position;

        IEvent::Ptr event(new ComponentAdded(ID, componentID));
        pEventManager->SendEvent<ComponentAdded>(event);
    }

    template<class ComponentType>
    inline ComponentType* Entity::GetComponent()
    {
        GL_CHECK(IsActive, "Attempt to use a disabled entity.");

        const auto positionInPool = GetPositionInPool<ComponentType>();
        ComponentType* component = static_cast<ComponentType*>(pComponentPools->GetComponent<ComponentType>(positionInPool));

        return component;
    }

    template<class ComponentType>
    inline void Entity::RemoveComponent()
    {
        GL_CHECK(IsActive, "Attempt to use a disabled entity.");

        const auto positionInPool = GetPositionInPool<ComponentType>();
        pComponentPools->RemoveComponent<ComponentType>(positionInPool);
        Components.erase(ComponentIdFromType::Get<ComponentType>());

        IEvent::Ptr event(new ComponentRemoved(ID, ComponentIdFromType::Get<ComponentType>()));
        pEventManager->SendEvent<ComponentRemoved>(event);
    }

    inline void Entity::RemoveAllComponents()
    {
        for (auto it = Components.begin(), end = Components.end(); it != end; )
        {
            pComponentPools->RemoveComponent(it->first, it->second);

            IEvent::Ptr event(new ComponentRemoved(ID, it->first));
            Components.erase(it++);
            pEventManager->SendEvent<ComponentRemoved>(event);
        }
    }

    template<class ComponentType>
    PositionInPool Entity::GetPositionInPool()
    {
        const ComponentID componentID = ComponentIdFromType::Get<ComponentType>();
        GL_CHECK(Components.find(componentID) != Components.end(), "Unable to get a component's position in pool: the component hasn't been added to entity.");
        const PositionInPool positionInPool = Components[componentID];

        return positionInPool;
    }
}
