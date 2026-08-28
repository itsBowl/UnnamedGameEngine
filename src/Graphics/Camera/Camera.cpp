#include "PCH.hpp"
#include "IO/InputListener.hpp"
#include "Camera.hpp"
#include "Logging/Log.hpp"
#include "Graphics/GraphicsFactory.hpp"


namespace EngineCore
{
    static const std::string LOGGER_TAG = "GraphicsCamera";
    Camera::Camera(InputHandler& i, Window& w) :input(i), window(w)
    {
        /* old mouse code, keeping as ref for how events can be set up
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
        */
        
        mouseClickEvent = InputListener(&input, (EngineCore::ListenerID)input.onMousePressed([this](const MouseEvent& e)
        {
            if (e.button == SDL_BUTTON_RIGHT)
            {
                rotate = true;
            }
        }));

        mouseReleaseEvent = InputListener(&input, (EngineCore::ListenerID)input.onMouseReleased([this](const MouseEvent& e)
        {
            if (e.button == SDL_BUTTON_RIGHT)
            {
                rotate = false;
            }
        }));

        mouseMoveEvent = InputListener(&input, (EngineCore::ListenerID)input.onMouseMoved([this](const MouseMoveEvent& e)
        {
            if (!rotate) return;

            yaw += e.dx;
            pitch -= e.dy;
            pitch = glm::clamp(pitch, -89.f, 89.f);
        }));
        
        createUBO();

    }

    void Camera::createUBO()
    {
        ubo = GraphicsFactory::createUniformBuffer();
    }

    void Camera::updateUBO()
    {
        uboData.view = view;
        uboData.proj = projection;
        uboData.pos = glm::vec4(position, 1.0);

        ubo->setData(&uboData, sizeof(UBO), 0);
    }


    void Camera::onUpdate(float dT)
    {
        Log::info(LOGGER_TAG, "Camera Update");
        float speed = 3.f; // get frametime

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
        up = glm::normalize(glm::cross(side, forward));

        moveDirection = glm::vec3(0.f);
        //update these so they can be set by the user? or maybe that's a thing for the player controller?
        if (input.isKeyHeld(SDL_SCANCODE_W)) moveDirection += forward;
        if (input.isKeyHeld(SDL_SCANCODE_S)) moveDirection -= forward;
        if (input.isKeyHeld(SDL_SCANCODE_D)) moveDirection += side;
        if (input.isKeyHeld(SDL_SCANCODE_A)) moveDirection -= side;
        if (input.isKeyHeld(SDL_SCANCODE_Q)) moveDirection += up;
        if (input.isKeyHeld(SDL_SCANCODE_E)) moveDirection -= up;
        if (glm::length(moveDirection) >= 0.05f)
        {
            moveDirection = glm::normalize(moveDirection);
        }

        position += moveDirection * speed * dT;
        
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