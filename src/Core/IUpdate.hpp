#pragma once



namespace EngineCore
{
    class IUpdate
    {
    public:
        IUpdate();
        virtual ~IUpdate();
        virtual void onUpdate(float dt) = 0;
    };
}