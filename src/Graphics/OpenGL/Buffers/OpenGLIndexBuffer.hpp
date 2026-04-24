#pragma once
#include "PCH.hpp"
#include "GL/gl3w.h"


namespace EngineCore
{
    class IndexBuffer
    {
    public:
        void create(const uint32_t* idx, uint32_t count, GLenum usage = GL_STATIC_DRAW);
        void bind() const;
        void unbind() const;
        void destory();
        uint32_t getCount() const { return count; }
        bool exists() const { return id != 0; }
    private:
        uint32_t id = 0;
        uint32_t count = 0;
    };
}