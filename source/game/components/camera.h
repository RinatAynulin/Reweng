#pragma once

#include <ecs/component.h>
#include <game/components/transform.h>

namespace Reweng
{
    struct CameraComponent : public IComponent
    {
        TransformComponent Transform;
        
        glm::mat4 View;
        glm::mat4 Projection;
    };
}