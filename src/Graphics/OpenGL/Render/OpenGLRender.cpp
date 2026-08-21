#include "OpenGLRender.hpp"
#include "Logger2.hpp"
#include <GL/gl3w.h>
#include "Asset/Mesh/Mesh.hpp"
#include "Graphics/Shader/IShader.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "OpenGLRender";


    OpenGLRender::~OpenGLRender()
    {
        Log::info("OpenGLRender destoryed");
        Log::flush();
    }

    void OpenGLRender::init()
    {
        Log::info(LOGGER_TAG, "Initialising OpenGLRender");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        stats.drawCalls = 0;
        stats.indexCount = 0;
        

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

    void OpenGLRender::draw(Mesh& m, std::shared_ptr<IShader> shader)
    {
        draw(m.getVAO(), shader);
    }

    void OpenGLRender::draw(std::shared_ptr<IVertexArray> vao, std::shared_ptr<IShader> shader, uint32_t indexCount)
    {
        shader->bind();
        vao->bind();

        uint32_t count = (indexCount == 0) ? vao->getIndexCount() : indexCount;
        
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

        ++stats.drawCalls;
        stats.indexCount += count;
    }

    void OpenGLRender::draw(std::shared_ptr<Mesh> m, std::shared_ptr<IShader> shader)
    {
        draw(m->getVAO(), shader);
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

    void OpenGLRender::setPipelineState(const PipelineState& p)
    {
        if (p == pipelineState)
            return;
        pipelineState = p;
        pipelineState.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        pipelineState.blending ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, pipelineState.wireframe ? GL_LINE : GL_FILL);
    }

    const PipelineState& OpenGLRender::getPipelineState() const
    {
        return pipelineState;
    }
}