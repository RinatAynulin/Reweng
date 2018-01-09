#include "pipelinemanager.h"

namespace Reweng
{
    void GLPipelineManager::Init()
    {
        ConstructGeneralPipeline();
    }

    void GLPipelineManager::ConstructGeneralPipeline()
    {
        GLShaderSettings vertexSettings = {};
        vertexSettings.ShaderFileLocation = "shaders/vertex.vert";
        vertexSettings.ShaderType = GL_VERTEX_SHADER;
        GLShader vertexShader(vertexSettings);

        vertexSettings.ShaderFileLocation = "shaders/fragment.frag";
        vertexSettings.ShaderType = GL_FRAGMENT_SHADER;
        GLShader fragmentShader(vertexSettings);

        GLShaderProgramCreateInfo programInfo = {};
        programInfo.VertexShader = vertexShader;
        programInfo.FragmentShader = fragmentShader;
        
        Pipelines[GRAPHICS_GENERAL].Init(programInfo);
    }
}