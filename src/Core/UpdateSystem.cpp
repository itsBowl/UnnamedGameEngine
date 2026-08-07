#include "UpdateSystem.hpp"
#include "IUpdate.hpp"
#include <algorithm>

namespace EngineCore
{
    void UpdateSystem::registerUpdate(IUpdate* u)
    {
        updatable.push_back(u);
    }

    void UpdateSystem::deregister(IUpdate* u)
    {
        updatable.erase(
            std::remove(updatable.begin(), updatable.end(), u),
            updatable.end()
        );
    }

    void UpdateSystem::updateAll(float dt)
    {
        //this can be expanded upon later to order things correctly (ie, do phys, gameplay, camera)
        for (auto* u : updatable)
        {
            u->onUpdate(dt);
        }
    }
}