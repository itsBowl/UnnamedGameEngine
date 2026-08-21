#include "MeshLibrary.hpp"
#include "MeshImporter.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Mesh Library";

    void MeshLibrary::add(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& mesh)
    {
        if (exists(name)) 
        {
            Log::warn(LOGGER_TAG, "Mesh already exists: ", name); 
            return;
        }
        meshes[name] = mesh;
    }

    std::vector<std::shared_ptr<Mesh>> MeshLibrary::load(const std::string& path)
    {
        if (exists(path))
            return get(path);

        //add the model loader here
        std::vector<std::shared_ptr<Mesh>> meshes = MeshImporter::import(path);

        if (meshes.empty())
        {
            Log::error(LOGGER_TAG, "failed to import mesh from: ", path);
            return meshes;
        }
        add(path, meshes);
        return meshes;

    }

    std::vector<std::shared_ptr<Mesh>> MeshLibrary::get(const std::string& name)
    {
        std::unordered_map<std::string, std::vector<std::shared_ptr<Mesh>>>::const_iterator it = meshes.find(name);

        if (it == meshes.end())
        {
            Log::error(LOGGER_TAG, "Mesh ", name, "not found");
            return std::vector<std::shared_ptr<Mesh>>();
        }

        return it->second;
    }

    bool MeshLibrary::exists(const std::string& name) const
    {
        return meshes.find(name) != meshes.end();
    }

    void MeshLibrary::debug()
    {
        for (std::pair<std::string, std::vector<std::shared_ptr<Mesh>>> m : meshes)
        {
            Log::info(LOGGER_TAG, m.first, "Submesh count: ", m.second.size());
        }
        Log::flush();
    }

    void MeshLibrary::deleteAll()
    {
        meshes.clear();
    }
}