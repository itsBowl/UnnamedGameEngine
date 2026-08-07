#pragma once
#include <cstdint>
#include <memory>
#include <glm/glm.hpp>
#include "Buffers/IVertexArray.hpp"

namespace EngineCore
{
    class Mesh;

    class IRender
    {
    public:
        virtual ~IRender() = default;

        virtual void init() = 0;
        virtual void shutdown() = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void setClearColour(const glm::vec4& c) = 0;
        virtual void clear() = 0;

        virtual void draw(Mesh& m) = 0;
        virtual void draw(std::shared_ptr<IVertexArray> vao, uint32_t idx = 0) = 0;
        virtual void drawArrays(std::shared_ptr<IVertexArray>, uint32_t verts) = 0;

        virtual void setViewport(int x, int y, int w, int h) = 0;
        virtual void setDepthTest(bool e) = 0;
        virtual void setBlending(bool e) = 0;
        virtual void setWireframe(bool e) = 0;

        struct Stats
        {
            uint32_t drawCalls = 0;
            uint32_t indexCount = 0;
        };
        virtual const Stats& getStats() const = 0;
    };
}