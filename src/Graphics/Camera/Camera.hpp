#pragma once

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "IO/InputHandler.hpp"
#include "IO/InputListener.hpp"
#include "Window/Window.hpp"
#include "gl/gl3w.h"
#include "Core/IUpdate.hpp"


namespace EngineCore
{
    enum class ProjectionType { Orthographic = 0, Perspective = 1};

    class Camera : public IUpdate
    {
    public:
        
        Camera(InputHandler&, Window&);
        void onUpdate(float dT) override;
        void updateUBO();
        

        glm::mat4& getView() {return view;}
        glm::mat4& getProj() {return projection;}
    private:

        void createUBO();
        

        //Entity* parent
        //glm::vec3 relativePosition
        //possibly make these a different camera class that is attached to players so I can have multiple things
        glm::mat4 projection;
        glm::mat4 view;

        GLuint ubo = 0;

        struct UBO
        {
            alignas(16)glm::mat4 view      = glm::mat4(1);
            alignas(16)glm::mat4 proj      = glm::mat4(1);
            alignas(16)glm::mat4 viewProj  = glm::mat4(1);
            alignas(16)glm::vec4 pos       = glm::vec4(1);
        } uboData;
        
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);

        glm::vec3 forward;
        glm::vec3 side;
        glm::vec3 up;

        float yaw = 0.f;
        float pitch = 0.f;
        
        float fov = 60.f;
        float sensitivity = 0.05f;
        bool rotate = false;

        InputHandler& input;
        Window& window;

        InputListener mouseMoveEvent;
        InputListener mouseClickEvent;
        InputListener mouseReleaseEvent;
        InputListener keyPressEvent;
        InputListener keyReleaseEvent;

        glm::vec2 mPos;
        glm::vec3 moveDirection = glm::vec3(0);

        

    };
}
