#include "Texture2d.hpp"
#include "Logger2.hpp"
#include "Errors.hpp"
#include <stb_image/stb_image.h>

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Texture";

    int Texture2d::load(const std::string& path)
    {
        stbi_set_flip_vertically_on_load(true); //openGL requirement

        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            Log::error(LOGGER_TAG, "Failed to load texture: ", path, " -- ", stbi_failure_reason());
            return TextureErrors::TEXTURE_FAILED_TO_LOAD;
        }

        GLenum internalFormat = GL_RGB8;
        GLenum dataFormat = GL_RGB;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
                     width, height, 0,
                     dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        Log::info(LOGGER_TAG, "Loaded texture id: ", id, " from path: ", path);

        return TextureErrors::TEXTURE_OK;
    }

    void Texture2d::bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture2d::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2d::destory()
    {
        if (exists())
        {
            Log::info(LOGGER_TAG, "Destorying texture: ", id);
            glDeleteTextures(1, &id);
            id = 0;
        }
    }
}