#pragma once
#include "glm/glm.hpp"

namespace EngineCore
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };
}