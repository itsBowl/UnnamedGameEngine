#pragma once

#include "GL/gl3w.h"

namespace EngineCore
{
    class GeometryBuffer
    {
    public:
        GLuint buffer;

    private:
        GLuint RT0;
        GLuint RT1;
        GLuint RT2;
        GLuint depth;


    };
}

