#pragma once
#include <vector>


namespace EngineCore
{
    class IUpdate;
    class UpdateSystem
    {
    public: 
        void registerUpdate(IUpdate* u);
        void deregister(IUpdate* u);
        void updateAll(float dt);
    private:
        std::vector<IUpdate*> updatable;
    };
}