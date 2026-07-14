#pragma once

namespace EngineCore
{

    class UpdateSystem;


    class IUpdate
    {
    public:
        IUpdate(UpdateSystem& us);
        virtual ~IUpdate();
        virtual void onUpdate(float dt) = 0;
    private:
        UpdateSystem& system;
    };
}