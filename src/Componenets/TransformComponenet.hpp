#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace EngineCore
{
    struct TransformComponent
    {
        glm::vec3 position = glm::vec3{0.f};
        glm::vec3 rotation = glm::vec3{0.f}; 
        glm::vec3 scale = glm::vec3{1.f};

        glm::mat4 getMatrix() const
        {
            glm::mat4 m = glm::translate(glm::mat4(1.f), position);
            m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
            m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
            m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
            return glm::scale(m, scale);
        }
    };
}