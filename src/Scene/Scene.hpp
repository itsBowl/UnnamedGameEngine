#pragma once
#include <entt/entt.hpp>

namespace EngineCore
{
    class Scene
    {
    public:
        entt::entity createEntity()
        {
            return registry.create();
        }

        void destoryEntity(entt::entity e)
        {
            registry.destroy(e);
        }

        entt::registry& getRegistry() { return registry; }

    private:
        entt::registry registry;
    };
}