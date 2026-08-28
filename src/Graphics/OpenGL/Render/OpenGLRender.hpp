#pragma once
#include "Render/IRender.hpp"

namespace EngineCore
{
    class OpenGLRender : public IRender
    {
    public:
        void init(const WindowHandle& wh = {});
        void shutdown() override;
        ~OpenGLRender() override;

        void beginFrame() override;
        void endFrame() override;

        void setClearColour(const glm::vec4& c) override;
        void clear() override;

        void draw(Mesh& m, std::shared_ptr<IShader> shader, const std::vector<std::shared_ptr<IUniformBuffer>> ubos = {}) override;
        void draw(std::shared_ptr<Mesh> m, std::shared_ptr<IShader> shader, const std::vector<std::shared_ptr<IUniformBuffer>> ubos = {}) override;
        void draw(std::shared_ptr<IVertexArray> vao, std::shared_ptr<IShader> shader, const std::vector<std::shared_ptr<IUniformBuffer>> ubos = {}, uint32_t indexCount = 0) override;
        void drawArrays(std::shared_ptr<IVertexArray> vao, uint32_t vertexCount, const std::vector<std::shared_ptr<IUniformBuffer>> ubos = {}) override;

        void setViewport(int x, int y, int w, int h) override;
        void setPipelineState(const PipelineState& s) override;
        const PipelineState& getPipelineState() const override;

        const Stats& getStats() const override { return stats; }

    private:
        glm::vec4 clearColour = glm::vec4{1.f, 0.4f, .2f, 1.f};
        Stats stats {0, 0};
        PipelineState pipelineState;
    };
}