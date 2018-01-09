#pragma once

#include <thirdparty/vulkan/vulkan.h>

#include <string>
#include <vector>

namespace Reweng
{
    struct GLSL;
    struct SPIRV;

    template<class Syntax>
    struct ShaderFile
    {
        ShaderFile(const std::string& location)
            : Location(location)
        {
        }
        std::string Location;
    };

    class ShaderLoader
    {
    public:
        explicit ShaderLoader(VkDevice logicalDevice);
        VkShaderModule LoadShader(const ShaderFile<GLSL>& shaderFile) const;
    
    private:
        VkDevice LogicalDevice;
    };
}