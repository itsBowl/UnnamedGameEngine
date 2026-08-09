#pragma once
#include "Render/IRender.hpp"

namespace EngineCore
{
    class OpenGLRender : public IRender
    {
    public:
        void init() override;
        void shutdown() override;

        void beginFrame() override;
        void endFrame() override;

        void setClearColour(const glm::vec4& c) override;
        void clear() override;

        void draw(Mesh& m) override;
        void draw(std::shared_ptr<IVertexArray> vao, uint32_t indexCount = 0) override;
        void drawArrays(std::shared_ptr<IVertexArray> vao, uint32_t vertexCount) override;

        void setViewport(int x, int y, int w, int h) override;
        void setDepthTest(bool enabled) override;
        void setBlending(bool enabled) override;
        void setWireframe(bool enabled) override;

        const Stats& getStats() const override { return stats; }

    private:
        glm::vec4 clearColour = glm::vec4{1.f, 0.4f, .2f, 1.f};
        Stats stats;
    };
}