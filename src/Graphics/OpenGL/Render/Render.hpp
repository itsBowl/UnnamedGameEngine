#pragma once

#include <cstdint>
#include <GL/gl3w.h>
#include "Graphics/OpenGL/Buffers/OpenGLVertexArray.hpp"
#include "Graphics/OpenGL/Shader/Shader.hpp"

namespace EngineCore
{
    class Render
    {
    public:
        void init();
        void shutdown();

        void beginFrame();
        void endFrame();

        void setClearColour(const glm::vec4& c);
        void clear();

        void drawIndexed(const VertexArray& vao, uint32_t indexCount = 0);
        void drawArrays(const VertexArray& var, uint32_t vertexCount);

        void setViewport(int x, int y, int w, int h);
        void setDepthTest(bool enabled);
        void setBlending(bool enabled);
        void setWireframe(bool enabled);

        struct Stats
        {
            uint32_t drawCalls = 0;
            uint32_t indexCount = 0;
        };
        const Stats& getStats() const { return stats; }

    private:
        glm::vec3 clearColour = glm::vec4{1.f, 0.4f, .2f, 1.f};
        Stats stats;
    };
}