#include "RenderSystem.hpp"
#include "Componenets/TransformComponenet.hpp"
#include "Componenets/MeshComponent.hpp"
#include "Componenets/MaterialComponent.hpp"
#include "Componenets/RenderDataComponent.hpp"

#include "Graphics/Camera/Camera.hpp"

namespace EngineCore
{

    static const std::string LOGGER_TAG = "Render";

    RenderSystem::RenderSystem()
    {
        
    }
    void RenderSystem::onUpdate(Scene& s, IRender& r, Camera& c)
    {
        entt::registry& reg = s.getRegistry();

        entt::basic_view view = reg.view<TransformComponent, MeshComponent, MaterialComponent>();

        std::shared_ptr<IUniformBuffer> camera = c.getUBO();

        std::vector<glm::mat4> matricies;
        matricies.reserve(maxEntities);

        for (entt::entity e : view)
        {
            matricies.push_back(view.get<TransformComponent>(e).getMatrix());
        }

        cbModel->setData(matricies.data(), static_cast<uint32_t>(matricies.size()), sizeof(glm::mat4));

        for (entt::entity e : view)
        {
            TransformComponent& transform = view.get<TransformComponent>(e);
            MeshComponent& mesh = view.get<MeshComponent>(e);
            MaterialComponent& mat = view.get<MaterialComponent>(e);

            if (!mat.shader)
            {
                continue;
            }

            for (std::shared_ptr<Mesh>& m : mesh.meshes)
            {
                r.draw(m, mat.shader, {camera});
            }
        }
    }

    uint32_t RenderSystem::allocateModelIndex()
    {
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
}