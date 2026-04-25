#pragma once
#include "gl/gl3w.h"
#include "Buffers/OpenGLBufferLayout.hpp"
#include "Buffers/OpenGLIndexBuffer.hpp"
#include "Buffers/OpenGLVertexBuffer.hpp"
#include "Buffers/OpenGLVertexArray.hpp"
#include "Shader/Shader.hpp"

namespace EngineCore
{
    class Render3d
    {
    public:
        Render3d() {};
        ~Render3d() = default;

        void preprocessing();

        VertexArray& getTri();
        void bindShader();
        void bindArray() { triArray.bind(); }
        void renderTri();
        void setupTri();

    private:
        Shader shader;
        VertexArray triArray;
        VertexBuffer triBuffer;
        IndexBuffer triIndex;
        
    };
}