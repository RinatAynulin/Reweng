#include "renderer.h"

#include <renderer/glshader.h>
#include <common/error.h>

#include <thirdparty/glm/gtc/matrix_transform.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>

namespace Reweng
{
    void Renderer::Init(WindowHolder::Ptr pWindow)
    {                
        this->pWindow = pWindow;
        InitializeGLAD();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        const auto windowExtent = pWindow->GetExtent();
        glViewport(0, 0,windowExtent.Width, windowExtent.Height);
        MeshManager.Init();
    }

    void Renderer::Flush()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (const auto& mesh : SubmitedMeshes)
        {
            mesh.Material.Shader.Use();
            glBindTexture(GL_TEXTURE_2D, mesh.Material.Texture.TextureID);
            glBindVertexArray(mesh.VAO);            

            glm::mat4 model = glm::translate(glm::mat4(), mesh.Transform.Position) * mesh.Transform.RotationMatrix;

            mesh.Material.Shader.SetUniformMatrix4fv("model", glm::value_ptr(model));
            mesh.Material.Shader.SetUniformMatrix4fv("view", glm::value_ptr(CurrentCamera.View));
            mesh.Material.Shader.SetUniformMatrix4fv("projection", glm::value_ptr(CurrentCamera.Projection));

            glDrawElements(GL_TRIANGLES, mesh.IndiceCount, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(pWindow->GetHandle());
        SubmitedMeshes.resize(0);
    }

    void Renderer::InitializeGLAD()
    {
        int err = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        GL_CHECK(err, "Cannot initialize GLAD.");
    }
}
