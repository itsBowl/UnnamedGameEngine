#pragma once
enum CoreErrors
{
    CORE_OK = 0,
    SDL_INIT_FAIL,
    SDL_FAILED_TO_CREATE_CONTEXT,
    GRAPHICS_FAILED_TO_INIT,
    
};

enum GraphicsErrors
{
    GRAPHICS_OK = 0,
    GRAPHICS_GL3W_FAILED_TO_INIT,

};

enum ShaderErrors
{
    SHADER_OK = 0,
    SHADER_FAILED_READ_VS,
    SHADER_FAILED_READ_FS,
    SHADER_FAILED_COMPILE,
    SHADER_FAILED_LINK,
};

enum TextureErrors
{
    TEXTURE_OK = 0,
    TEXTURE_FAILED_TO_LOAD,
};