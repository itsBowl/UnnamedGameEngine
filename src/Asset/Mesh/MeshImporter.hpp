#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Asset/Vertex.hpp"
#include "Mesh.hpp"


struct aiScene;
struct aiMesh;
struct aiNode;

namespace EngineCore
{
    class MeshImporter
    {
    public:
        static std::vector<std::shared_ptr<Mesh>> import(const std::string& path);
    private:
        static void processNode(const aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
        static std::shared_ptr<Mesh> processMesh(const aiMesh* mesh);
    };
}