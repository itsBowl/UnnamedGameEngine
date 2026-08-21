#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Mesh.hpp"

namespace EngineCore
{
    class MeshLibrary
    {
    public:
        void add(const std::string& name, const std::vector<std::shared_ptr<Mesh>>&);

        std::vector<std::shared_ptr<Mesh>> load(const std::string& path);
        std::vector<std::shared_ptr<Mesh>> get(const std::string& name);

        bool exists(const std::string& name) const;
        void debug();
        void deleteAll();

    private:
        std::unordered_map<std::string, std::vector<std::shared_ptr<Mesh>>> meshes;
    };
}