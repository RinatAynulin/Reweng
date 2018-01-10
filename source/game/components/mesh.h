#pragma once

#include <ecs/component.h>
#include <game/components/transform.h>
#include <game/components/material.h>

#include <thirdparty/glad/glad.h>

namespace Reweng
{
    struct MeshComponent : IComponent
    {
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        GLuint IndiceCount;

        TransformComponent Transform;
        MaterialComponent  Material;
    };
}