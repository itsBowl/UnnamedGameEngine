#include "IUpdate.hpp"
#include "Locator.hpp"
#include "UpdateSystem.hpp"

namespace EngineCore
{
    IUpdate::IUpdate()
    {
        Locator::getUpdateSystem().registerUpdate(this);
    }

    IUpdate::~IUpdate()
    {
        Locator::getUpdateSystem().deregister(this);
    }
}
