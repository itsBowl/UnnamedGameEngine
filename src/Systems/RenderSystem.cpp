#include "RenderSystem.hpp"
#include "Componenets/TransformComponenet.hpp"
#include "Componenets/MeshComponent.hpp"
#include "Componenets/MaterialComponent.hpp"
#include "Componenets/RenderDataComponent.hpp"

#include "Graphics/Camera/Camera.hpp"
#include "Graphics/GraphicsFactory.hpp"
#include "Graphics/GraphicsAPI.hpp"

#include "OpenGL/Buffers/OpenGLStructuredBuffer.hpp"
#include "OpenGL/Buffers/OpenGLUniformBuffer.hpp"
#ifdef DX_12_SUPPORT
#include "DirectX12/Buffers/DirectX12StructuredBuffer.hpp"
#include "DirectX12/Buffers/DirectX12UniformBuffer.hpp"
#endif

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Render";

    RenderSystem::RenderSystem()
    {
        
    }

    RenderSystem::RenderSystem(Scene& s, IRender& r)
    {

    }

    RenderSystem::~RenderSystem()
    {

    }

    void RenderSystem::init()
    {
        
        cbModel = GraphicsFactory::createStructedBuffer(1024, sizeof(glm::mat4));
        modelIndexBuffer = GraphicsFactory::createUniformBuffer(sizeof(DrawData));
    }

    void RenderSystem::onUpdate(Scene& s, IRender& r, Camera& c)
    {
        //needs to be swapped to a key system later for perf
        batches.clear();
        packed.clear();
        entt::registry& reg = s.getRegistry();

        entt::basic_view view = reg.view<TransformComponent, MeshComponent, MaterialComponent>();
        std::shared_ptr<IUniformBuffer> camera = c.getUBO();

        //Gather batches for dispatch
        for (entt::entity e : view)
        {
            MaterialComponent& mat = view.get<MaterialComponent>(e);
            if (!mat.shader) continue;

            glm::mat4 t = view.get<TransformComponent>(e).getMatrix();

            for (std::shared_ptr<Mesh>& m : view.get<MeshComponent>(e).meshes)
            {

                std::pair<Mesh*, IShader*> key {m.get(), mat.shader.get()};

                Batch& b = batches[key];
                b.mesh = m;
                b.shader = mat.shader;
                b.transforms.push_back(t);
                b.hash = createDebugHash(b.mesh.get(), b.shader.get());

                std::unordered_map<std::pair<Mesh*, IShader*>, std::shared_ptr<IUniformBuffer>, PairHash>::iterator it = drawBuffers.find(key);
                if (it == drawBuffers.end())
                {
                    std::shared_ptr<IUniformBuffer> buf = GraphicsFactory::createUniformBuffer(sizeof(uint32_t) * 2);
                    drawBuffers[key] = buf;
                    b.batchDataBuffer = buf;
                }
                else
                {
                    b.batchDataBuffer = it->second;
                }
            }
        }

        for (std::pair<const std::pair<Mesh*, IShader*>, Batch>& e : batches)
        {
            e.second.base = static_cast<uint32_t>(packed.size());
            packed.insert(packed.end(), e.second.transforms.begin(), e.second.transforms.end());
        }

        cbModel->setData(packed.data(), static_cast<uint32_t>(packed.size()), sizeof(glm::mat4));
        
        r.frameData(camera, cbModel);

        for (std::pair<const std::pair<Mesh*, IShader*>, Batch>& e : batches)
        {
            Batch& b = e.second;
            //Log::info(LOGGER_TAG, "Batch: ", b.base, " ", b.hash);
            DrawData d{b.base, b.hash};
            b.batchDataBuffer->setData(&d, sizeof(d));
            r.drawInstances(b.mesh, b.shader, {b.batchDataBuffer}, static_cast<uint32_t>(b.transforms.size()));
        }
        

        /*
        this is the old way things were done
        now its batched and instanced, so things should be done with less draw calls
        good change
        for (entt::entity e : view)
        {
            TransformComponent& transform = view.get<TransformComponent>(e);
            MeshComponent& mesh = view.get<MeshComponent>(e);
            MaterialComponent& mat = view.get<MaterialComponent>(e);
            RenderDataComponent& rdc = view.get<RenderDataComponent>(e);
            modelIndexBuffer->setData(&rdc.bufferIndex, sizeof(rdc.bufferIndex));
            

            if (!mat.shader)
            {
                continue;
            }
            
            for (std::shared_ptr<Mesh>& m : mesh.meshes)
            {
                r.draw(m, mat.shader, {modelIndexBuffer});
            }
        }
        */
    }

    uint32_t RenderSystem::allocateModelIndex()
    {
        Log::info(LOGGER_TAG, "Allocating model Index: ", next);
        if (!freeIndices.empty())
        {
            uint32_t index = freeIndices.back();
            freeIndices.pop_back();
            return index;
        }

        if (next >= maxEntities)
        {
            Log::error(LOGGER_TAG, "Exceeded maximum entity count for the model buffer");
            return 0;
        }
        
        return next++;
    }

    void RenderSystem::releaseBufferIndex(uint32_t i)
    {
        freeIndices.push_back(i);
    }

    void RenderSystem::onRenderDataDestoryed(entt::registry& r, entt::entity e)
    {
        releaseBufferIndex(r.get<RenderDataComponent>(e).bufferIndex);
    }

    uint32_t RenderSystem::createDebugHash(Mesh* m, IShader* s)
    {
        uint32_t mh = cascadeHash(static_cast<uint32_t>(reinterpret_cast<uint64_t>(m)));
        uint32_t sh = cascadeHash(static_cast<uint32_t>(reinterpret_cast<uint64_t>(s)));
        return cascadeHash(mh + sh * 0x9e3779b9U) % 4096;
    }

    uint32_t RenderSystem::cascadeHash(uint32_t h)
    {
        h ^= h >> 16;
        h *= 0x7feb352dU;
        h ^= h >> 15;
        h *= 0x846ca68bU;
        h ^= h >> 16;
        return h;
    }
}