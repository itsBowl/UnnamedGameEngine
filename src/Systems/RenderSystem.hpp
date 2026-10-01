#pragma once
#include <map>
#include "Scene/Scene.hpp"
#include "Render/IRender.hpp"
#include "Buffers/IStructuredBuffer.hpp"
#include "Buffers/IUniformBuffer.hpp"

#include "Asset/Mesh/Mesh.hpp"
#include "Shader/IShader.hpp"



namespace EngineCore
{
    class Camera;
    struct Batch
    {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<IShader> shader;
        std::vector<glm::mat4> transforms;
        uint32_t base = 0;
        uint32_t hash = 0;
        std::shared_ptr<IUniformBuffer> batchDataBuffer;
    };

    class RenderSystem
    {
    public:
        RenderSystem();
        RenderSystem(Scene& s, IRender& r);
        ~RenderSystem();
        void init();
        uint32_t allocateModelIndex();
        void releaseBufferIndex(uint32_t i);
        void onUpdate(Scene& s, IRender& r, Camera& c);
        void onRenderDataDestoryed(entt::registry& r, entt::entity e);
    private:
        struct DrawData
        {
            uint32_t base;
            uint32_t hash;
        };

        struct PairHash
        {
            size_t operator()(const std::pair<Mesh*, IShader*>& key) const
            {
                uint64_t a = reinterpret_cast<uint64_t>(key.first);
                uint64_t b = reinterpret_cast<uint64_t>(key.second);
                uint64_t h = a ^ (b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2));
                return static_cast<size_t>(h ^ (h >> 32));
            }
        };
        std::shared_ptr<IStructuredBuffer> cbModel;
        std::shared_ptr<IUniformBuffer> modelIndexBuffer;
        std::vector<uint32_t> freeIndices;
        static const uint32_t maxEntities = 1024;
        uint32_t next = 0;
        std::map<std::pair<Mesh*, IShader*>, Batch> batches;
        std::unordered_map<std::pair<Mesh*, IShader*>, std::shared_ptr<IUniformBuffer>, PairHash> drawBuffers;
        std::vector<glm::mat4> packed;
        
        uint32_t createDebugHash(Mesh* m, IShader* s);
        uint32_t cascadeHash(uint32_t h);
    };
}