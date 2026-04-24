#include "InputHandler.hpp"

namespace EngineCore
{

    static const std::string LOGGER_TAG = "INPUT HANDLER";

   void InputHandler::init()
   {
        heldKeys.clear();
        heldMouse.clear();
        mouseX = 0.f;
        mouseY = 0.f;
        idCounter = 0;
   }

   void InputHandler::processEvent(const SDL_Event& e)
   {
        switch (e.type)
        {
            case SDL_EVENT_KEY_DOWN:
            {
                heldKeys[e.key.scancode] = true;
                KeyEvent event{e.key.scancode, e.key.key, e.key.repeat};
                keyPressed.dispatch(event);
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                heldKeys[e.key.scancode] = false;
                KeyEvent event {e.key.scancode, e.key.key, false};
                keyReleased.dispatch(event);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                heldMouse[e.button.button] = true;
                MouseEvent event {e.button.button, e.button.x, e.button.y};
                mousePressed.dispatch(event);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                heldMouse[e.button.button] = false;
                MouseEvent event {e.button.button, e.button.x, e.button.y};
                mouseReleased.dispatch(event);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                MouseMoveEvent event{mouseX, mouseY, e.motion.xrel, e.motion.yrel};
                mouseMoved.dispatch(event);
                break;
            }
            default:
                break;
        }
    }

    ListenerID InputHandler::onKeyPressed(std::function<void(const KeyEvent&)> listener)
    {
        ListenerID id = ++idCounter;
        keyPressed.add(id, std::move(listener));
        return id;
    }

    ListenerID InputHandler::onKeyReleased(std::function<void(const KeyEvent&)> listener)
    {
        ListenerID id = ++idCounter;
        keyReleased.add(id, std::move(listener));
        return id;
    }

    ListenerID InputHandler::onMousePressed(std::function<void(const MouseEvent&)> listener)
    {
        ListenerID id = ++idCounter;
        mousePressed.add(id, std::move(listener));
        return id;
    }

    ListenerID InputHandler::onMouseReleased(std::function<void(const MouseEvent&)> listener)
    {
        ListenerID id = ++idCounter;
        mouseReleased.add(id, std::move(listener));
        return id;
    }

    ListenerID InputHandler::onMouseMoved(std::function<void(const MouseMoveEvent&)> listener)
    {
        ListenerID id = ++idCounter;
        mouseMoved.add(id, std::move(listener));
        return id;
    }

    ListenerID InputHandler::onMouseScroll(std::function<void(const MouseScrollEvent&)> listener)
    {
        ListenerID id = ++idCounter;
        mouseScroll.add(id, std::move(listener));
        return id;
    }

    void InputHandler::removeListener(ListenerID id)
    {
        //this is kinda bad for removeing listeners but it work
        if (keyPressed.remove(id)) return;
        if (keyReleased.remove(id)) return;
        if (mousePressed.remove(id)) return;
        if (mouseReleased.remove(id)) return;
        if (mouseMoved.remove(id)) return;
        if (mouseScroll.remove(id)) return;

        logWarn(LOGGER_TAG, "attempted to remove a listener with an unknown id: ", id);
    }

    bool InputHandler::isKeyHeld(SDL_Scancode sc) const
    {
        auto it = heldKeys.find(sc);
        return it != heldKeys.end() && it->second;
    }

    bool InputHandler::isMouseButtonHeld(uint8_t mb) const
    {
        auto it = heldMouse.find(mb);
        return it != heldMouse.end() && it->second;
    }

    float InputHandler::getMouseX() const { return mouseX; }
    float InputHandler::getMouseY() const { return mouseY; }

}