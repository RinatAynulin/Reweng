#pragma once

#include <thirdparty/vulkan/vulkan.h>
#include <thirdparty/glm/glm.hpp>

#include <array>

namespace Reweng
{
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        typedef  std::array<VkVertexInputAttributeDescription, 2> VertexInputAttributeDescriptions;

        static VkVertexInputBindingDescription GetInputBindingDescription()
        {
            VkVertexInputBindingDescription binding{};
            binding.binding = 0;
            binding.stride = sizeof(Vertex);
            binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return binding;
        }

        static VertexInputAttributeDescriptions GetInputAttributeDescriptions()
        {
            VertexInputAttributeDescriptions attributes = {};
            attributes[0].location = 0;
            attributes[0].binding = 0;
            attributes[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributes[0].offset = 0;

            attributes[1].location = 1;
            attributes[1].binding = 0;
            attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributes[1].offset = offsetof(Vertex, color);

            return attributes;
        }
    };
}