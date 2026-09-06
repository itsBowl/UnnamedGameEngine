#include "MeshImporter.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Mesh Importer";

    std::shared_ptr<Mesh> MeshImporter::processMesh(const aiMesh* mesh)
    {
        std::vector<Vertex> verts;
        std::vector<uint32_t> index;

        verts.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex v;

            const aiVector3D& pos = mesh->mVertices[i];
            v.position = glm::vec3(pos.x, pos.y, pos.z);

            v.normal = glm::vec3(0.f, 0.f, 1.f);
            if (mesh->HasNormals())
            {
                const aiVector3D& n = mesh->mNormals[i];
                v.normal = glm::vec3(n.x, n.y, n.z);
            }

            v.uv = glm::vec2(0.f, 0.f);
            if(mesh->HasTextureCoords(0))
            {
                const aiVector3D& u = mesh->mTextureCoords[0][i];
                v.uv = glm::vec2(u.x, u.y);
            }
            verts.push_back(v);
        }

        index.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);

        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& f = mesh->mFaces[i];
            for (unsigned int j = 0; j < f.mNumIndices; j++)
            {
                index.push_back(f.mIndices[j]);
            }
        }

        std::shared_ptr<Mesh> _mesh = std::make_shared<Mesh>();
        _mesh->create(verts, index);

        return _mesh;
    }

    void MeshImporter::processNode(const aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]]));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, meshes);
        }
    }

    std::vector<std::shared_ptr<Mesh>> MeshImporter::import(const std::string& path)
    {
        Log::info(LOGGER_TAG, "Importing mesh from: ", path);
        Log::flush();
        std::vector<std::shared_ptr<Mesh>> meshes;

        Assimp::Importer importer;

        const unsigned int flags = 
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace;

        const aiScene* scene = importer.ReadFile(path, flags);

        if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 || scene->mRootNode == nullptr)
        {
            Log::error(LOGGER_TAG, "Assimp failed to laod: ", path, " ", importer.GetErrorString());
            return meshes;
        }

        processNode(scene->mRootNode, scene, meshes);

        Log::info(LOGGER_TAG, "Imported: ", meshes.size(), "meshes from: ", path);

        return meshes;

    }
}