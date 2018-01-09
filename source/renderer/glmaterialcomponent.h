#pragma once

#include <renderer/gltexture.h>
#include <renderer/glshader.h>

namespace Reweng
{
    struct GLMaterialComponent
    {
        GLShader shader;
        GLTexture texture;
    };
}