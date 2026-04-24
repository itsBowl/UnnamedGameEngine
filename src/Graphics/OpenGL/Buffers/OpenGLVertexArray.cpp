#include "OpenGLVertexArray.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Vertex Array";

    //Creates a vertex array, does not bind
    void VertexArray::create()
    {
        glGenVertexArrays(1, &id);
        logInfo(LOGGER_TAG, "Created VAO: ", id);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(id);
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(const VertexBuffer& vbo, const BufferLayout& layout)
    {
        bind();
        vbo.bind();

        for (const auto& e : layout.getElements())
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
        }

    }

    void VertexArray::setIndexBuffer(const IndexBuffer& ibo)
    {
        bind();
        ibo.bind();
        indexCount = ibo.getCount();
    }

    void VertexArray::destroy()
    {
        if(exists())
        {
            logInfo(LOGGER_TAG, "Destorying VAO: ", id);
            glDeleteVertexArrays(1, &id);
            id = 0;
        }
    }
}