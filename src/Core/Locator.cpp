#include "Locator.hpp"
#include "UpdateSystem.hpp"
#include "IO/InputHandler.hpp"
#include "Graphics/Render/IRender.hpp"
#include "Time.hpp"

namespace EngineCore
{
    UpdateSystem* Locator::updateSystem = nullptr;
    InputHandler* Locator::inputHandler = nullptr;
    IRender* Locator::render = nullptr;
    Time* Locator::time = nullptr;

    void Locator::provide(UpdateSystem* us) {updateSystem = us;}
    void Locator::provide(InputHandler* ih) {inputHandler = ih;}
    void Locator::provide(IRender* r) {render = r;}
    void Locator::provide(Time* t) {time = t;}

    //These should have some asserts to check not null, add later
    UpdateSystem& Locator::getUpdateSystem() {return *updateSystem;}
    InputHandler& Locator::getInputHandler() {return *inputHandler;}
    IRender& Locator::getRenderer() {return *render;}
    Time& Locator::getTime() {return *time;}
}