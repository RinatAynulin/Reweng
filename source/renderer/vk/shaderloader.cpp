#include "shaderloader.h"

#include <common/error.h>

#include <fstream>

namespace Reweng
{
    ShaderLoader::ShaderLoader(VkDevice logicalDevice)
        : LogicalDevice(logicalDevice)
    {
    }

    VkShaderModule ShaderLoader::LoadShader(const ShaderFile<GLSL>& shaderFile) const
    {
        std::ifstream file(shaderFile.Location, std::ios::ate | std::ios::binary);
        if (file.is_open() == false)
            throw Common::Error(DBG_LINE, "Unable to load a shader file", Common::Critical);

        const size_t size = file.tellg();
        VK_CHECK(size > 0, "Unable to load a shader file: size = 0");
        file.seekg(std::ios::beg);

        std::vector<char> shaderData(size);
        file.read(shaderData.data(), size);

        VkShaderModuleCreateInfo shaderModuleInfo = {};
        shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleInfo.codeSize = size;
        shaderModuleInfo.pCode = (uint32_t*)shaderData.data();

        VkShaderModule shaderModule;
        VK_VALIDATE(vkCreateShaderModule(LogicalDevice, &shaderModuleInfo, nullptr, &shaderModule));
        return shaderModule;
    }
}