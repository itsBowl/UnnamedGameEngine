#pragma once

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "IO/InputHandler.hpp"
#include "IO/InputListener.hpp"
#include "Window/Window.hpp"


namespace EngineCore
{
    enum class ProjectionType { Orthographic = 0, Perspective = 1};

    class Camera
    {
    public:
        
        Camera(InputHandler&, Window&);
        void onUpdate();
        

        glm::mat4& getView() {return view;}
        glm::mat4& getProj() {return projection;}
    private:

        //Entity* parent
        //glm::vec3 relativePosition
        //possibly make these a different camera class that is attached to players so I can have multiple things
        glm::mat4 projection;
        glm::mat4 view;
        
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);

        glm::vec3 forward;
        glm::vec3 side;
        glm::vec3 up;

        float yaw;
        float pitch;
        
        float fov = 60.f;
        float sensitivity = 0.05f;

        InputHandler& input;
        Window& window;

        InputListener mouseMoveEvent;
        InputListener mouseClickEvent;
        InputListener keyPressEvent;
        InputListener keyReleaseEvent;

        glm::vec2 mPos;
        glm::vec3 moveDirection = glm::vec3(0);

        

    };
}
