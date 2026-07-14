#pragma once

#include <glm/glm.hpp>


namespace EngineCore
{
    enum class ProjectionType { Orthographic = 0, Perspective = 1};

    class Camera
    {
    public:
        
        Camera() = default;
        Camera(const glm::mat4& p) : projection(p) {}
        virtual ~Camera() = default;

        const glm::mat4& getProjection() const { return projection; }
    private:
        glm::mat4 projection = glm::mat4(1.f);
    };
}