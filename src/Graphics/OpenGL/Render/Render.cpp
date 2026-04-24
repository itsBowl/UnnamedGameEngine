#include "Render.hpp"
#include "Logger2.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Render";

    void Render::init()
    {
        logInfo(LOGGER_TAG, "Initialising Render");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        

        flushLogs();
    }

    void Render::shutdown()
    {
        logInfo(LOGGER_TAG, "Render shutdown");
        flushLogs();
    }

    void Render::beginFrame()
    {
        stats.drawCalls = 0;
        stats.indexCount = 0;
    }

    void Render::endFrame()
    {
        //figure all this out later
    }

    void Render::setClearColour(const glm::vec4& colour)
    {
        clearColour = colour;
        glClearColor(colour.r, colour.g, colour.b, colour.a);
    }

    void Render::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Render::drawIndexed(const VertexArray& vao, uint32_t indexCount)
    {
        vao.bind();

        uint32_t count = (indexCount == 0) ? vao.getIndexCount() : indexCount;

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

        stats.drawCalls++;
        stats.indexCount += count;
    }

    void Render::drawArrays(const VertexArray& vao, uint32_t vertexCount)
    {
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        stats.drawCalls++;
    }

    void Render::setViewport(int x, int y, int w, int h)
    {
        glViewport(x, y, w, h);
    }

    void Render::setDepthTest(bool enabled)
    {
        enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void Render::setBlending(bool enabled)
    {
        enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    }

    void Render::setWireframe(bool enabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
    }
}