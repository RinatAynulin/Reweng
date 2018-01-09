#pragma once

#include <thirdparty/glad/glad.h>

#include <vector>
#include <string>
#include <optional>

namespace Reweng
{
    struct GLShaderCreateInfo
    {
        std::string_view                VertexShaderFSLocation;
        std::string_view                FragmentShaderFSLocation;
        std::optional<std::string_view> TesselationShaderFSLocation;
        std::optional<std::string_view> GeometryShaderFSLocation;
    };

    /*Current class is a presentation of a fully compiled shader pipeline aka Shader Program*/
    class GLShader final
    {
    public:
        GLShader() = default;
        explicit GLShader(const GLShaderCreateInfo& settings);
        
        inline GLShader(const GLShader& shader)
        {
            ID = shader.ID;
        }

        inline GLShader(GLShader&& shader)
        {
            ID = shader.ID;
            shader.ID = 0;
        }

        GLShader& operator=(const GLShader& shader) = default;

        inline void Use() const
        {
            glUseProgram(ID);
        }

        inline void SetUniformMatrix4fv(const char* uniformName, GLfloat* data, GLboolean transpose = GL_FALSE) const
        {
            GLuint uniformLocation = glGetUniformLocation(ID, uniformName);
            glUniformMatrix4fv(uniformLocation, 1, transpose, data);
        }

        inline operator GLuint()
        {
            return ID;
        }

        inline operator GLuint() const
        {
            return ID;
        }

    private:
        GLuint            CreateShader(std::string_view shaderLocation, GLuint shaderType) const;
        std::vector<char> LoadShader(std::string_view ShaderFileLocation) const;
        void              CheckCompilationStatus(GLuint shaderID) const;
        void              CheckLinkageStatus() const;
    private:
        GLuint ID = 0;
    };
}