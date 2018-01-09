#pragma once
#include <renderer/glshadermanager.h>
#include <renderer/modelmanager.h>
#include <renderer/gltexturemanager.h>

#include <thirdparty/glad/glad.h>
#include <thirdparty/glfw3/glfw3.h>

#include <array>
#include <vector>

namespace Reweng
{
    enum ShaderProgramTypes
    {
        SHADER_PROGRAM_GENERAL = 0,
        PROGRAM_COUNT = (SHADER_PROGRAM_GENERAL + 1)
    };

    struct Extent2D
    {
        uint16_t Width, Height;
    };

    struct RendererSettings
    {
        struct
        {
            const char* Name;
            Extent2D Extent;
        } Window;
    };

    class Renderer
    {
    public:
        void Init(const RendererSettings& settings);       

        inline void Submit(const GLMeshComponent& mesh)
        {
            SubmitedMeshes.push_back(mesh);
        }

        void Flush();

    public:
        GLMeshManager     MeshManager;
        GLTextureManager  TextureManager;
        GLShaderManager   ShaderManager;

    private:
        void InitializeGLFW();
        void ConstructWindow(Extent2D extent, const char* tittle);
        void InitializeGLAD();

    private:
        GLFWwindow* Window = nullptr;
        Extent2D    WindowExtent = {};        

        std::vector<GLMeshComponent> SubmitedMeshes;
    };
}