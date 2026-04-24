#pragma once

#include <string>
#include <cstdint>
#include <GL/gl3w.h>

namespace EngineCore
{
    class Texture2d
    {
    public:
        int load(const std::string& path);
        void bind(uint32_t slot = 0) const;
        void unbind() const;
        void destory();

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        bool exists() const { return id != 0; }
        uint32_t getID() const { return id; }

    private:
        uint32_t id = 0;
        int width = 0;
        int height = 0;
        int channels = 0;
    };
}