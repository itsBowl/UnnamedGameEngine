#include "IUpdate.hpp"
#include "UpdateSystem.hpp"

namespace EngineCore
{
    IUpdate::IUpdate(UpdateSystem& us)
    : system(us)
    {
        system.registerUpdate(this);
    }

    IUpdate::~IUpdate()
    {
        system.deregister(this);
    }
}