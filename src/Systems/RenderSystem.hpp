#pragma once
#include "Scene/Scene.hpp"
#include "Render/IRender.hpp"
#include "Buffers/IStructuredBuffer.hpp"

class Camera;

namespace EngineCore
{
    class RenderSystem
    {
    public:
        RenderSystem();
        ~RenderSystem();
        uint32_t allocateModelIndex();
        void releaseBufferIndex(uint32_t i);
        void onUpdate(Scene& s, IRender& r, Camera& c);
        void onRenderDataDestoryed(entt::registry& r, entt::entity e);
    private:
        std::shared_ptr<IStructuredBuffer> cbModel;
        std::vector<uint32_t> freeIndices;
        static const uint32_t maxEntities = 1024;
        uint32_t next = 0;
    };
}