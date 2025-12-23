#pragma once

namespace EngineCore
{
    class GraphicsContext
    {
    public:
        virtual int init() = 0;
        virtual void swapBuffers() = 0;
    };
}