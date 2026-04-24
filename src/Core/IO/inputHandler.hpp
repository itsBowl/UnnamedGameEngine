#pragma once

#include "PCH.hpp"
#include "SDL3/SDL.h"

namespace EngineCore
{

    struct KeyEvent
    {
        SDL_Scancode scancode;
        SDL_Keycode keycode;
        bool repeat;
    };

    struct MouseEvent
    {
        uint8_t button;
        float x, y;
    };

    struct MouseMoveEvent
    {
        float x, y;
        float dx, dy;
    };

    struct MouseScrollEvent
    {
        float dx, dy;
    };

    using ListenerID = uint32_t;



    class InputHandler
    {
    public:
        void init();
        
        void processEvent(const SDL_Event& e);

        ListenerID onKeyPressed(std::function<void(const KeyEvent&)>);
        ListenerID onKeyReleased(std::function<void(const KeyEvent&)>);
        ListenerID onMousePressed(std::function<void(const MouseEvent&)>);
        ListenerID onMouseReleased(std::function<void(const MouseEvent&)>);
        ListenerID onMouseMoved(std::function<void(const MouseMoveEvent&)>);
        ListenerID onMouseScroll(std::function<void(const MouseScrollEvent&)>);

        void removeListener(ListenerID id);

        bool isKeyHeld(SDL_Scancode sc) const;
        bool isMouseButtonHeld(uint8_t bt) const;
        float getMouseX() const;
        float getMouseY() const;

        private:

            ListenerID idCounter = 0;
        template<typename TEvent>
        struct Channel
        {
            std::unordered_map<ListenerID, std::function<void(const TEvent&)>> listeners;

            void add(ListenerID id, std::function<void(const TEvent&)> listener)
            {
                listeners[id] = std::move(listener);
            }

            ListenerID subscribe(std::function<void(const TEvent&)> cb, ListenerID& count)
            {
                ListenerID id = ++idCounter;
                listeners[id] = std::move(cb);
                return id;
            }

            void dispatch(const TEvent& e) const
            {
                for (auto& [id, cb] : listeners) cb(e);
            }

            bool remove(ListenerID id) { return listeners.erase(id) > 0; }
        };

        Channel<KeyEvent> keyPressed;
        Channel<KeyEvent> keyReleased;
        Channel<MouseEvent> mousePressed;
        Channel<MouseEvent> mouseReleased;
        Channel<MouseMoveEvent> mouseMoved;
        Channel<MouseScrollEvent> mouseScroll;

        std::unordered_map<SDL_Scancode, bool> heldKeys;
        std::unordered_map<uint8_t, bool> heldMouse;
        float mouseX = 0.f, mouseY = 0.f;

        

    };
}