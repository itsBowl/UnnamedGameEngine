#pragma once

#include "glm/glm.hpp"

namespace EngineCore
{
    enum class ProjectionType { Orthographic = 0, Perspective = 1};

    class EditorCamera
    {
    public:
        //Entity* parent
        //glm::vec3 relativePosition
        //possibly make these a different camera class that is attached to players so I can have multiple things
        struct Matricies
        {
            glm::mat4 projection;
            glm::mat4 view;
        } matricies;
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);

        glm::vec3 forward;
        glm::vec3 side;
        glm::vec3 up;

        glm::mat4& getView() {return matricies.view;}
        glm::mat4& getProj() {return matricies.projection;}

        

    };
}
