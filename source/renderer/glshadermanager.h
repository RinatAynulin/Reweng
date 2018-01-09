#pragma once

#include <common/error.h>
#include <renderer/glshader.h>

#include <thirdparty/glad/glad.h>

#include <string_view>
#include <unordered_map>
#include <vector>

namespace Reweng
{
    typedef std::string_view ShaderName;
    typedef GLuint ShaderID;

    class GLShaderManager final
    {
    public:
        inline ~GLShaderManager()
        {
            for (auto& shader : Shaders)
                glDeleteProgram(shader);
        }

        void UploadShader(const GLShaderCreateInfo& infos, std::string_view shaderName);

        inline GLShader GetShader(ShaderID id)
        {
            return Shaders[id];
        }

        inline GLShader GetShader(ShaderName name)
        {
            const auto it = ShaderMap.find(name);
            GL_CHECK(it != ShaderMap.end(), "Unable to get the required shader.");

            return Shaders[it->second];
        }

        inline ShaderID GetShaderID(ShaderName name)
        {
            const auto it = ShaderMap.find(name);
            GL_CHECK(it != ShaderMap.end(), "Unable to get shader id by the given name.");

            return it->second;
        }

    private:
        std::vector<GLShader> Shaders;

        std::unordered_map<ShaderName, ShaderID> ShaderMap;
    };
}