#pragma once
#include <game/components/mesh.h>
#include <game/components/camera.h>
#include <renderer/glshadermanager.h>
#include <renderer/modelmanager.h>
#include <renderer/gltexturemanager.h>
#include <renderer/windowholder.h>

#include <thirdparty/glad/glad.h>
#include <thirdparty/glfw3/glfw3.h>

#include <array>
#include <vector>
#include <memory>

namespace Reweng
{
    enum ShaderProgramTypes
    {
        SHADER_PROGRAM_GENERAL = 0,
        PROGRAM_COUNT = (SHADER_PROGRAM_GENERAL + 1)
    };

    class Renderer
    {
    public:
        typedef std::shared_ptr<Renderer> Ptr;

    public:
        void                         Init(WindowHolder::Ptr pWindow);       
                                     
        inline void                  Submit(const MeshComponent& mesh);        
        void                         Flush();
                                     
        inline void                  SetCamera(const CameraComponent& camera);
        inline Extent2D              GetWindowExtent() const;
                                     
    public:                          
        GLMeshManager                MeshManager;
        GLTextureManager             TextureManager;
        GLShaderManager              ShaderManager;
                                     
    private:                         
        void                         InitializeGLAD();

    private:
        WindowHolder::Ptr            pWindow;

        std::vector<MeshComponent>   SubmitedMeshes;
        CameraComponent              CurrentCamera;
    };

    inline void Renderer::Submit(const MeshComponent& mesh)
    {
        SubmitedMeshes.push_back(mesh);
    }

    inline void Renderer::SetCamera(const CameraComponent& camera)
    {
        CurrentCamera = camera;
    }

    inline Extent2D Renderer::GetWindowExtent() const
    {
        return pWindow->GetExtent();
    }
}