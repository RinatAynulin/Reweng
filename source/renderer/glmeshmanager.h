#pragma once

#include <common/error.h>
#include <renderer/vertex.h>
#include <game/components/mesh.h>

#include <thirdparty/glad/glad.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <optional>

namespace Reweng
{
    typedef GLuint MeshID;

    class GLMeshManager
    {
    public:
        void Init();

        MeshID UploadModel(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

        inline const MeshComponent& GetModel(MeshID id)
        {
            return Meshes[id];
        }

        inline const MeshComponent& GetMesh(const std::string& name)
        {
            const auto it = MeshTable.find(name);
            GL_CHECK(it != MeshTable.end(), "Unable to get a model by name.");

            return Meshes[it->second];
        }

    private:
        void UploadCubeModel();

    private:
        std::vector<MeshComponent> Meshes;
        std::unordered_map<std::string, MeshID> MeshTable;
    };
}