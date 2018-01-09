#pragma once

#include <renderer/glshader.h>
#include <renderer/glmaterialcomponent.h>
#include <renderer/gltransformcomponent.h>

#include <thirdparty/glad/glad.h>

namespace Reweng
{
    struct GLMeshComponent
    {
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;

        GLuint indiceCount;

        GLMaterialComponent material;
        GLTransformComponent transform;
    };
}