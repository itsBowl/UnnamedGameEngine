#pragma once
#include <cstdint>
#include <memory>
#include <glm/glm.hpp>
#include "Buffers/IVertexArray.hpp"
#include "Shader/IShader.hpp"
#include "Buffers/IUniformBuffer.hpp"

namespace EngineCore
{
    class Mesh;
    class IShader;

    struct PipelineState
    {
        bool depthTest = true;
        bool backfaceCull = true;
        bool blending = false;
        bool wireframe = false;

        bool operator==(const PipelineState& other) const
        {
            return depthTest == other.depthTest &&
                blending == other.blending &&
                wireframe == other.wireframe &&
                backfaceCull == other.backfaceCull;
        }
    };

    struct WindowHandle
    {
        void* handle = nullptr;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    class IRender
    {
    public:
        virtual ~IRender() = default;

        virtual void init(const WindowHandle& = {}) = 0;
        virtual void shutdown() = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void setClearColour(const glm::vec4& c) = 0;
        virtual void clear() = 0;

        //These functions ran off the end of the screen q_q
        virtual void draw(Mesh& m, std::shared_ptr<IShader> shader, 
            const std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers = {}
        ) = 0;
        virtual void draw(std::shared_ptr<Mesh> m, std::shared_ptr<IShader> shader, 
            const std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers = {}
        ) = 0;
        virtual void draw(std::shared_ptr<IVertexArray> vao, std::shared_ptr<IShader> shader, 
            const std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers = {}, 
            uint32_t idx = 0
        ) = 0;
        virtual void drawArrays(std::shared_ptr<IVertexArray>, uint32_t verts,
            const std::vector<std::shared_ptr<IUniformBuffer>> ubos = {}
        ) = 0;

        virtual void setViewport(int x, int y, int w, int h) = 0;

        virtual void setPipelineState(const PipelineState& s) = 0;
        virtual const PipelineState& getPipelineState() const = 0;

        struct Stats
        {
            uint32_t drawCalls = 0;
            uint32_t indexCount = 0;
        };
        virtual const Stats& getStats() const = 0;
    };
}