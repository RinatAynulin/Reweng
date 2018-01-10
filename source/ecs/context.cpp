#include "context.h"

#include <ecs/entityevents.h>

namespace Reweng
{
    Context::Context()
        : pEventManager(new EventManager())
        , entityCount(0)
        , pComponentPools(new ComponentPools(10))
    {
        pEventManager->RegisterEvent<EntityAdded>();
        pEventManager->RegisterEvent<EntityRemoved>();
        pEventManager->RegisterEvent<ComponentAdded>();
        pEventManager->RegisterEvent<ComponentRemoved>();
    }

    void Context::Loop()
    {
        while (true)
        {
            for (auto& system : LogicSystems)
            {
                system->Update(0);
            }
        }
    }

    Entity::Ptr Context::CreateEntity()
    {
        if (ReusableEntities.empty() == false)
        {
            const EntityID reusableID = ReusableEntities.top();
            GL_CHECK(Entities[reusableID]->IsActivated() == false, "Attempt to reuse an activated entity.");

            Entities[reusableID]->Enable();
            ReusableEntities.pop();
            return Entities[reusableID];
        }

        Entity::Ptr newEntity{ new Entity(pEventManager, pComponentPools, entityCount++) };

        Entities.push_back(newEntity);

        IEvent::Ptr event(new EntityAdded(newEntity));
        pEventManager->SendEvent<EntityAdded>(event);

        return newEntity;
    }

    void Context::Step(const double dt)
    {
        //for (auto& system : Systems)
          //  system->Update(dt);
    }
}