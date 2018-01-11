#pragma once

#include <ecs/component.h>

#include <thirdparty/glm/glm.hpp>

namespace Reweng
{
    struct TransformComponent : public IComponent
    {
        glm::vec3 Position;
        glm::mat4 RotationMatrix;
        
        glm::vec3 Scale;
    };
}