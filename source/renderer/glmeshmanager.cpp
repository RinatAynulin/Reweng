#include "glmeshmanager.h"

namespace Reweng
{
    void GLMeshManager::Init()
    {
        UploadCubeModel();
    }

    MeshID GLMeshManager::UploadModel(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
    {
        GL_CHECK(vertices.size() != 0 && indices.size() != 0, "Unable to upload a model.");

        MeshComponent mesh = {};

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

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texturePosition)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        MeshID id = Meshes.size();
        MeshTable[name] = id;
        Meshes.push_back(mesh);
        
        return id;
    }
    //TODO: in shader rewrite attribute positions
    void GLMeshManager::UploadCubeModel()
    {
        std::vector<Vertex> vertices = {
          { { -0.5, -0.5,  0.5 } ,{ 0.0f, 0.0f } },
          { {  0.5, -0.5,  0.5 } ,{ 1.0f, 0.0f } },
          { {  0.5,  0.5,  0.5 } ,{ 1.0f, 1.0f } },
          { { -0.5,  0.5,  0.5 } ,{ 0.0f, 1.0f } },
          { { -0.5, -0.5, -0.5 } ,{ 0.0f, 0.0f } },
          { {  0.5, -0.5, -0.5 } ,{ 1.0f, 0.0f } },
          { {  0.5,  0.5, -0.5 } ,{ 1.0f, 1.0f } },
          { { -0.5,  0.5, -0.5 } ,{ 0.0f, 1.0f } },
        };

        std::vector<GLuint> indices = {
            // front
            0, 1, 2,
            2, 3, 0,
            // top
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // bottom
            4, 0, 3,
            3, 7, 4,
            // left
            4, 5, 1,
            1, 0, 4,
            // right
            3, 2, 6,
            6, 7, 3,
        };

        MeshComponent mesh = {};

        glGenVertexArrays(1, &mesh.VAO);        
        glBindVertexArray(mesh.VAO);

        glGenBuffers(1, &mesh.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &mesh.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
        mesh.IndiceCount = static_cast<GLuint>(indices.size());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texturePosition)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        MeshID id = Meshes.size();
        MeshTable["cube"] = id;
        Meshes.push_back(mesh);
    }
}