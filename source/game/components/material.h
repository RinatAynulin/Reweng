#pragma once

#include <ecs/component.h>
#include <renderer/gltexture.h>
#include <renderer/glshader.h>

namespace Reweng
{
    struct MaterialComponent : public IComponent
    {
        GLShader shader;
        GLTexture texture;
    };
}