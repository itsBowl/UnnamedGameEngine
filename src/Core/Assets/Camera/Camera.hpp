#pragma once

#include <glm/glm.hpp>
#include "Core/IUpdate.hpp"

namespace EngineCore
{
    enum class ProjectionType { Orthographic = 0, Perspective = 1};

    class aCamera : public IUpdate
    {
    public:
        
        aCamera() = default;
        aCamera(const glm::mat4& p) : projection(p) {}
        virtual ~aCamera() = default;
        void onUpdate(float deltaTime) override;

        const glm::mat4& getProjection() const { return projection; }
    private:
        glm::mat4 projection = glm::mat4(1.f);
    };
}