#include "Locator.hpp"
#include "UpdateSystem.hpp"
#include "IO/InputHandler.hpp"
#include "Graphics/Render/IRender.hpp"
#include "Time.hpp"
#include "Asset/AssetManager.hpp"

namespace EngineCore
{
    UpdateSystem* Locator::updateSystem = nullptr;
    InputHandler* Locator::inputHandler = nullptr;
    IRender* Locator::render = nullptr;
    Time* Locator::time = nullptr;
    AssetManager* Locator::assets = nullptr;

    void Locator::provide(UpdateSystem* us) {updateSystem = us;}
    void Locator::provide(InputHandler* ih) {inputHandler = ih;}
    void Locator::provide(IRender* r) {render = r;}
    void Locator::provide(Time* t) {time = t;}
    void Locator::provide(AssetManager* a) {assets = a;}

    //These should have some asserts to check not null, add later
    UpdateSystem& Locator::getUpdateSystem() {return *updateSystem;}
    InputHandler& Locator::getInputHandler() {return *inputHandler;}
    IRender& Locator::getRenderer() {return *render;}
    Time& Locator::getTime() {return *time;}
    AssetManager& Locator::getAssetManager() { return *assets;}
}