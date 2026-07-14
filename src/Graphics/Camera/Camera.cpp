#include "PCH.hpp"
#include "IO/InputListener.hpp"
#include "Camera.hpp"


namespace EngineCore
{
    Camera::Camera(InputHandler& i, UpdateSystem& u, Window& w) : IUpdate(u), input(i), window(w)
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

    void Camera::createUBO()
    {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UBO), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
    }

    void Camera::updateUBO()
    {
        uboData.view = view;
        uboData.proj = projection;
        uboData.pos = glm::vec4(position, 1.0);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBO), &uboData);
    }


    void Camera::onUpdate()
    {

        float speedScalar = 0.03f; // get frametime
        const glm::vec2& mouse{input.getMouseX(), input.getMouseY()};
        glm::vec2 delta = mouse - mPos;

        mPos = mouse;

        glm::mat4 lookAt;
        forward = glm::vec3(0, 0, -1);
        up = glm::vec3(0, 1, 0);
        side = glm::vec3(1, 0, 0);

        glm::vec3 cameraFront = glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
        );
        
        forward = glm::normalize(cameraFront);
        side = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

        moveDirection = glm::vec3(0.f);
        //update these so they can be set by the user? or maybe that's a thing for the player controller?
        if (input.isKeyHeld(SDL_SCANCODE_W)) moveDirection += forward;
        if (input.isKeyHeld(SDL_SCANCODE_S)) moveDirection -= forward;
        if (input.isKeyHeld(SDL_SCANCODE_D)) moveDirection += side;
        if (input.isKeyHeld(SDL_SCANCODE_A)) moveDirection -= side;
        moveDirection = glm::normalize(moveDirection);

        position += moveDirection * speedScalar;
        

        

        //lookAt = glm::rotate(-yaw, glm::vec3(0, 1, 0)) * glm::rotate(-pitch, glm::vec3(1, 0, 0));
        //forward = lookAt * glm::vec4(forward, 1.f);
        //side = glm::normalize(glm::cross(up, forward));
        //up = glm::normalize(glm::cross(forward, up));

        

        //our movement is already handled by subscribing to the listeners when we created the camera
        //position += moveDirection;

        //move some of the magic numbers here to be relative to the current resolution, but for now it's fine
        projection = glm::perspective(glm::radians(fov), (float)16/(float)9, .1f, 100.f);
        view = glm::lookAt(position, position + forward, up);

        updateUBO();
    }
}