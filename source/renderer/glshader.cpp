#include <renderer/glshader.h>

#include <common/error.h>

#include <fstream>

namespace Reweng
{
    GLShader::GLShader(const GLShaderCreateInfo& settings)
    {
        ID = glCreateProgram();

        GLuint vertexShader = CreateShader(settings.VertexShaderFSLocation, GL_VERTEX_SHADER);
        glAttachShader(ID, vertexShader);
        glDeleteShader(vertexShader);

        GLuint fragmentShader = CreateShader(settings.FragmentShaderFSLocation, GL_FRAGMENT_SHADER);
        glAttachShader(ID, fragmentShader);
        glDeleteShader(fragmentShader);

        if (settings.TesselationShaderFSLocation.has_value())
        {
            GLuint tesselationShader = CreateShader(settings.TesselationShaderFSLocation.value(), GL_FRAGMENT_SHADER);
            glAttachShader(ID, tesselationShader);
            glDeleteShader(tesselationShader);
        }

        if (settings.GeometryShaderFSLocation.has_value())
        {
            GLuint geometryShader = CreateShader(settings.GeometryShaderFSLocation.value(), GL_GEOMETRY_SHADER);
            glAttachShader(ID, geometryShader);
            glDeleteShader(geometryShader);
        }

        glLinkProgram(ID);
        CheckLinkageStatus();
    }

    GLuint GLShader::CreateShader(std::string_view shaderLocation, GLuint shaderType) const
    {
        std::vector<char> shaderData = LoadShader(shaderLocation);
        GL_CHECK(shaderData.empty() != true, "Renderer encounterred a problems with loading a shader.");

        GLuint shaderID = glCreateShader(shaderType);

        const GLchar* const pShaderData = shaderData.data();
        const GLint size = static_cast<GLint>(shaderData.size());
        glShaderSource(shaderID, 1, &pShaderData, &size);
        glCompileShader(shaderID);

        CheckCompilationStatus(shaderID);

        return shaderID;
    }

    std::vector<char> GLShader::LoadShader(std::string_view shaderFileLocation) const
    {
        std::ifstream file(shaderFileLocation.data(), std::ios::ate | std::ios::binary);
        if (file.is_open() == false)
            throw Common::Error(DBG_LINE, "Unable to load a shader file", Common::Critical);

        const size_t size = file.tellg();
        GL_CHECK(size > 0, "Unable to load a shader file: size = 0");
        file.seekg(std::ios::beg);

        std::vector<char> shaderData(size);
        file.read(shaderData.data(), size);

        return shaderData;
    }

    void GLShader::CheckCompilationStatus(GLuint shaderID) const
    {
        int success = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char errMsg[512];
            glGetShaderInfoLog(shaderID, sizeof(errMsg), nullptr, errMsg);
            throw Common::Error(DBG_LINE, errMsg, Common::Critical);
        }
    }

    void GLShader::CheckLinkageStatus() const
    {
        int success = 0;
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char errMsg[512];
            glGetProgramInfoLog(ID, 512, NULL, errMsg);
            throw Common::Error(DBG_LINE, errMsg, Common::Critical);
        }
    }
}