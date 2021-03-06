#include "inputcleanupsystem.h"

#include <ecs/context.h>
#include <ecs/group.h>
#include <game/components/inputcomponents.h>

namespace Reweng
{
    InputCleanupSystem::InputCleanupSystem(std::shared_ptr<Context> pcontext)
        : CleanupSystem(pcontext)
    {
    }

    void InputCleanupSystem::Clean()
    {
        Group::Ptr inputGroup = pContext->GetGroup<InputKey>();
        for (const auto& entity : inputGroup->GetEntities())
        {
            if (entity == nullptr)
                continue;

            const EntityID id = entity->GetID();
            pContext->RemoveEntity(id);
        }
    }
}