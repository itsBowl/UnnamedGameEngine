#include "PCH.hpp"
#include "IO/InputListener.hpp"
#include "Camera.hpp"


namespace EngineCore
{
    Camera::Camera(InputHandler& i, Window& w) : input(i), window(w)
    {
        mouseMoveEvent = InputListener(&input, (EngineCore::ListenerID)input.onMouseMoved([this](const MouseMoveEvent& e)
        {
            yaw += e.dx * sensitivity;
            pitch -= e.dy * sensitivity;
        }));

        keyPressEvent = InputListener(&input, (EngineCore::ListenerID)input.onKeyPressed([this](const KeyEvent& e)
        {
            switch (e.scancode)
            {
                case SDL_SCANCODE_W: moveDirection += glm::vec3(1, 0, 0); break;
                case SDL_SCANCODE_S: moveDirection -= glm::vec3(1, 0, 0); break;
                case SDL_SCANCODE_A: moveDirection += glm::vec3(0, 0, 1); break;
                case SDL_SCANCODE_D: moveDirection -= glm::vec3(0, 0, 1); break;
            }
        }));

        keyReleaseEvent = InputListener(&input, (EngineCore::ListenerID)input.onKeyReleased([this](const KeyEvent& e)
        {
            switch (e.scancode)
            {
                case SDL_SCANCODE_W: moveDirection -= glm::vec3(1, 0, 0); break;
                case SDL_SCANCODE_S: moveDirection += glm::vec3(1, 0, 0); break;
                case SDL_SCANCODE_A: moveDirection -= glm::vec3(0, 0, 1); break;
                case SDL_SCANCODE_D: moveDirection += glm::vec3(0, 0, 1); break;
            }
        })); 

    }


    void Camera::onUpdate()
    {
        const glm::vec2& mouse{input.getMouseX(), input.getMouseY()};
        glm::vec2 delta = mouse - mPos;

        mPos = mouse;

        glm::mat4 lookAt;
        forward = glm::vec3(0, 0, -1);
        up = glm::vec3(0, 1, 0);
        side = glm::vec3(1, 0, 0);

        lookAt = glm::rotate(-yaw, glm::vec3(0, 1, 0)) * glm::rotate(-pitch, glm::vec3(1, 0, 0));

        forward = lookAt * glm::vec4(forward, 1.f);
        side = glm::normalize(glm::cross(up, forward));
        up = glm::normalize(glm::cross(forward, up));

        float speedScalar = 0.03f; // get frametime

        //our movement is already handled by subscribing to the listeners when we created the camera
        position += moveDirection;

        projection = glm::perspective(glm::radians(fov), (float)16/(float)9, .1f, 100.f);
        view = lookAt * glm::translate(position);
        


    }
}