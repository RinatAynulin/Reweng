#pragma once

#include <thirdparty/glm/glm.hpp>

namespace Reweng
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texturePosition;
    };
}