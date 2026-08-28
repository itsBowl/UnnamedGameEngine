#include "OpenGLVertexArray.hpp"
#include "Buffers/IVertexBuffer.hpp"
#include "Buffers/IIndexBuffer.hpp"
#include "OpenGLVertexBuffer.hpp"
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLDataTypes.hpp"


namespace EngineCore
{
    static const std::string LOGGER_TAG = "Vertex Array";

    OpenGLVertexArray::OpenGLVertexArray()
    {
        create();
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        destroy();
    }

    //Creates a vertex array, does not bind
    void OpenGLVertexArray::create()
    {
        glGenVertexArrays(1, &id);
        Log::info(LOGGER_TAG, "Created VAO: ", id);
    }

    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(id);
    }

    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(std::shared_ptr<IVertexBuffer> vbo, const BufferLayout& layout)
    {
        bind();
        static_cast<OpenGLVertexBuffer*>(vbo.get())->bind();

        for (const BufferElement& e : layout.getElements())
        {
            GLenum type = shaderDataTypeOpenGL(e.type);
            uint32_t comps = shaderDataTypeComponentCount(e.type);
            uint32_t stride = layout.getStride();

            if (type == GL_INT || type == GL_BOOL)
            {
                glVertexAttribIPointer(
                    attributeIndex,
                    comps, type, stride,
                    reinterpret_cast<const void*>(e.offset)
                );
            }
            else
            {
                glVertexAttribPointer(
                    attributeIndex,
                    comps, type, e.normalised ? GL_TRUE : GL_FALSE,
                    stride,
                    reinterpret_cast<const void*>(e.offset)
                );
            }
            glEnableVertexAttribArray(attributeIndex);
            attributeIndex++;
        }
        vertexBuffer = vbo;

    }

    void OpenGLVertexArray::addIndexBuffer(std::shared_ptr<IIndexBuffer> ibo)
    {
        bind();
        static_cast<OpenGLIndexBuffer*>(ibo.get())->bind();
        indexBuffer = ibo;
        indexCount = ibo->getCount();
    }

    void OpenGLVertexArray::destroy()
    {
        if(exists())
        {
            Log::info(LOGGER_TAG, "Destorying VAO: ", id);
            glDeleteVertexArrays(1, &id);
            id = 0;
        }
    }
}