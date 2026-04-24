#pragma once
#include "InputHandler.hpp"


namespace EngineCore
{
    class InputListener
    {
    public:
        InputListener() = default;
        InputListener(InputHandler* h, ListenerID i) : handler(h), id(i)
        {}

        bool exists() { return (handler && id != 0);}

        ~InputListener()
        {
            if (exists())
            {
                handler->removeListener(id);
            }
        }

        InputListener(const InputListener&) = delete;
        InputListener& operator=(const InputListener&) = delete;
        InputListener(InputListener&& other) noexcept
        {
            other.handler = nullptr;
            other.id = 0;
        }
        InputListener& operator=(InputListener&& other) noexcept
        {
            if (this != &other)
            {
                if (exists()) handler->removeListener(id);
                
                handler = other.handler;
                id = other.id;

                other.handler = nullptr;
                other.id = 0;
            }

            return *this;
        }

    private:
        InputHandler* handler = nullptr;
        ListenerID id = 0;
    };
}