#pragma once
#include "Texture/ITexture.hpp"
#include <string>
#include <cstdint>
#include <GL/gl3w.h>

namespace EngineCore
{
    class OpenGLTexture : public ITexture
    {
    public:
        OpenGLTexture() = default;
        OpenGLTexture(const std::string&);
        int load(const std::string& path) override;
        void bind(uint32_t slot = 0) const override;
        void unbind() override;
        void destory();

        int getWidth() override { return width; }
        int getHeight() override { return height; }
        bool exists() override { return id != 0; }
        const std::string& getName() const override;
        uint32_t getID() const { return id; }

    private:
        uint32_t id = 0;
        std::string name = "";
        int width = 0;
        int height = 0;
        int channels = 0;
    };
}