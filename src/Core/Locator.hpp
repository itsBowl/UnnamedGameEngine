#pragma once

namespace EngineCore
{
    class UpdateSystem;
    class InputHandler;
    class IRender;
    class Time;

    class Locator
    {
    public:
        static void provide(UpdateSystem* us);
        static void provide(InputHandler* ih);
        static void provide(IRender* r);
        static void provide(Time* t);

        static UpdateSystem& getUpdateSystem();
        static InputHandler& getInputHandler();
        static IRender& getRenderer(); 
        static Time& getTime();

    private:
        static UpdateSystem* updateSystem;
        static InputHandler* inputHandler;
        static IRender* render;
        static Time* time;
    };
}