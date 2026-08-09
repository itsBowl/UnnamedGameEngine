#include "OpenGLRender.hpp"
#include "Logger2.hpp"
#include <GL/gl3w.h>
#include "Asset/Mesh/Mesh.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "OpenGLRender";

    void OpenGLRender::init()
    {
        Log::info(LOGGER_TAG, "Initialising OpenGLRender");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        

        Log::flush();
    }

    void OpenGLRender::shutdown()
    {
        Log::info(LOGGER_TAG, "OpenGLRender shutdown");
        Log::flush();
    }

    void OpenGLRender::beginFrame()
    {
        stats.drawCalls = 0;
        stats.indexCount = 0;
    }

    void OpenGLRender::endFrame()
    {
        //figure all this out later
    }

    void OpenGLRender::setClearColour(const glm::vec4& colour)
    {
        clearColour = colour;
        glClearColor(colour.r, colour.g, colour.b, colour.a);
    }

    void OpenGLRender::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRender::draw(Mesh& m)
    {
        draw(m.getVAO());
    }

    void OpenGLRender::draw(std::shared_ptr<IVertexArray> vao, uint32_t indexCount)
    {
        vao->bind();

        uint32_t count = (indexCount == 0) ? vao->getIndexCount() : indexCount;

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

        stats.drawCalls++;
        stats.indexCount += count;
    }

    void OpenGLRender::drawArrays(std::shared_ptr<IVertexArray> vao, uint32_t vertexCount)
    {
        vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        stats.drawCalls++;
    }

    void OpenGLRender::setViewport(int x, int y, int w, int h)
    {
        glViewport(x, y, w, h);
    }

    void OpenGLRender::setDepthTest(bool enabled)
    {
        enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRender::setBlending(bool enabled)
    {
        enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    }

    void OpenGLRender::setWireframe(bool enabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
    }
}