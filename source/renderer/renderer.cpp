#include "renderer.h"

#include <renderer/glshader.h>
#include <common/error.h>

#include <thirdparty/glm/gtc/matrix_transform.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>

namespace Reweng
{
    void Renderer::Init(const RendererSettings& settings)
    {
        InitializeGLFW();
        ConstructWindow(settings.Window.Extent, settings.Window.Name);
        InitializeGLAD();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, settings.Window.Extent.Width, settings.Window.Extent.Height);
        MeshManager.Init();
    }

    void Renderer::Flush()
    {
        for (const auto& mesh : SubmitedMeshes)
        {
            mesh.material.shader.Use();
            glBindTexture(GL_TEXTURE_2D, mesh.material.texture.TextureID);
            glBindVertexArray(mesh.VAO);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(mesh.transform.position));
            glm::mat4 view;
            view = glm::translate(view, glm::vec3(0, 0, -3));
            glm::mat4 projection;
            projection = glm::perspective(45.0f, (float)WindowExtent.Width / (float)WindowExtent.Height, 0.1f, 100.0f);

            mesh.material.shader.SetUniformMatrix4fv("model", glm::value_ptr(model));
            mesh.material.shader.SetUniformMatrix4fv("view", glm::value_ptr(view));
            mesh.material.shader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36);//TODO: REMOVE AFTER TEST
        }

        glfwPollEvents();
        glfwSwapBuffers(Window);
    }

    void Renderer::InitializeGLFW()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    void Renderer::ConstructWindow(Extent2D extent, const char* title)
    {
        Window = glfwCreateWindow(extent.Width, extent.Height, title, nullptr, nullptr);
        GL_CHECK(Window != nullptr, "Unable to create a window.");
        glfwMakeContextCurrent(Window);
        WindowExtent = extent;
    }

    void Renderer::InitializeGLAD()
    {
        int err = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        GL_CHECK(err, "Cannot initialize GLAD.");
    }
}