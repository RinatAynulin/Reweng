#include "modelmanager.h"

namespace Reweng
{
    void GLMeshManager::Init()
    {
        UploadCubeMesh();
    }

    MeshID GLMeshManager::UploadMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
    {
        GL_CHECK(vertices.size() != 0 && indices.size() != 0, "Unable to upload a model.");

        GLMeshComponent mesh = {};

        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texturePosition)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        MeshID id = Meshes.size();
        MeshTable[name] = id;
        Meshes.push_back(mesh);
        
        return id;
    }

    void GLMeshManager::UploadCubeMesh()
    {
        std::vector<Vertex> vertices = {
            { { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { -0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },

            { { -0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { -0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },

            { { -0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { -0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
            { { -0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },

            { { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },

            { { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { -0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
            { { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },

            { { -0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
            { { -0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
            { { -0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
        };

        GLMeshComponent mesh = {};

        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);

        glBindVertexArray(mesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texturePosition)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        MeshID id = Meshes.size();
        MeshTable["Cube"] = id;
        Meshes.push_back(mesh);
    }
}