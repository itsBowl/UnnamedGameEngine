#include "OpenGLRender.hpp"
#include "Logger2.hpp"
#include <GL/gl3w.h>
#include "Asset/Mesh/Mesh.hpp"
#include "Graphics/Shader/IShader.hpp"
#include "OpenGL/Buffers/OpenGLVertexArray.hpp"
#include "Shader/OpenGLShader.hpp"
#include "Buffers/OpenGLUniformBuffer.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "OpenGLRender";


    OpenGLRender::~OpenGLRender()
    {
        Log::info("OpenGLRender destoryed");
        Log::flush();
    }

    void OpenGLRender::init(const WindowHandle& wh)
    {
        Log::info(LOGGER_TAG, "Initialising OpenGLRender");
        context.init(wh);

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
        context.swapBuffers();
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

    void OpenGLRender::draw(std::vector<std::shared_ptr<Mesh>> m, std::shared_ptr<IShader> shader, const std::vector<std::shared_ptr<IUniformBuffer>> ubos)
    {
        for (std::shared_ptr<Mesh> _m : m)
        {
            draw(_m, shader, ubos);
        }
    }

    void OpenGLRender::draw(Mesh& m, std::shared_ptr<IShader> shader, std::vector<std::shared_ptr<IUniformBuffer>> ubos)
    {
        draw(m.getVAO(), shader, ubos);
    }

    void OpenGLRender::draw(std::shared_ptr<IVertexArray> vao, std::shared_ptr<IShader> shader, 
        const std::vector<std::shared_ptr<IUniformBuffer>> ubos, uint32_t indexCount)
    {
        static_cast<OpenGLVertexArray*>(vao.get())->bind();
        static_cast<OpenGLShader*>(shader.get())->bind();

        for (uint32_t i = 0; i < ubos.size(); i++)
        {
            static_cast<OpenGLUniformBuffer*>(ubos[i].get())->bindToSlot(i);
        }

        uint32_t count = (indexCount == 0) ? vao->getIndexCount() : indexCount;
        
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

        stats.drawCalls++;
        stats.indexCount += count;
    }

    void OpenGLRender::draw(std::shared_ptr<Mesh> m, std::shared_ptr<IShader> shader, const std::vector<std::shared_ptr<IUniformBuffer>> ubos)
    {
        draw(m->getVAO(), shader, ubos);
    }

    void OpenGLRender::drawArrays(std::shared_ptr<IVertexArray> vao, uint32_t vertexCount, const std::vector<std::shared_ptr<IUniformBuffer>> ubos)
    {
        //legacy, needs removal
        static_cast<OpenGLVertexArray*>(vao.get())->bind();
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