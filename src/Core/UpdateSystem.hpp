#pragma once
#include <vector>
#include "IUpdate.hpp"

namespace EngineCore
{
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