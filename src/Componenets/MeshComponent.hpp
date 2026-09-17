#pragma once
#include <memory>
#include <vector>
#include <Asset/Mesh/Mesh.hpp>

namespace EngineCore
{
    struct MeshComponent
    {
        std::vector<std::shared_ptr<Mesh>> meshes;
    };
}