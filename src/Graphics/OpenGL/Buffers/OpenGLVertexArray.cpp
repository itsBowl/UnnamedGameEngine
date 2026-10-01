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
        OpenGLVertexBuffer* buf = static_cast<OpenGLVertexBuffer*>(vbo.get());

        //this swaps over to the size of the 
        uint32_t bindingIndex = static_cast<uint32_t>(vertexBuffers.size());

        glVertexArrayVertexBuffer(id, bindingIndex, buf->getID(), 0, layout.getStride());

        for (const BufferElement& e : layout.getElements())
        {
            GLenum type = shaderDataTypeOpenGL(e.type);
            uint32_t comps = shaderDataTypeComponentCount(e.type);
            uint32_t stride = layout.getStride();

            if (type == GL_INT || type == GL_BOOL)
            {
                // this is the old way of doing things
                // glVertexAttribIPointer(
                //     attributeIndex,
                //     comps, type, stride,
                //     reinterpret_cast<const void*>(e.offset)
                // );
                
                glVertexArrayAttribIFormat(
                    id,
                    attributeIndex,
                    comps,
                    type,
                    e.offset
                );
            }
            else
            {
                // old method
                // glVertexAttribPointer(
                //     attributeIndex,
                //     comps, type, e.normalised ? GL_TRUE : GL_FALSE,
                //     stride,
                //     reinterpret_cast<const void*>(e.offset)
                // );

                glVertexArrayAttribFormat(
                    id,
                    attributeIndex,
                    comps,
                    type,
                    e.normalised ? GL_TRUE : GL_FALSE,
                    e.offset
                );
            }
            glVertexArrayAttribBinding(id, attributeIndex, bindingIndex);
            glEnableVertexArrayAttrib(id, attributeIndex);
            attributeIndex++;
        }
        vertexBuffers.push_back(vbo);
        unbind();

    }

    void OpenGLVertexArray::addIndexBuffer(std::shared_ptr<IIndexBuffer> ibo)
    {
        indexBuffer = ibo;
        indexCount = ibo->getCount();
        glVertexArrayElementBuffer(id, static_cast<OpenGLIndexBuffer*>(ibo.get())->getID());
        
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