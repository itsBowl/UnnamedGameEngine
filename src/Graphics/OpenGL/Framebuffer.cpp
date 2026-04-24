#include "PCH.hpp"
#include "Framebuffer.hpp"

#include "gl/gl3w.h"

namespace EngineCore
{
    static const uint32_t maxFramebuffersize = 8192;
    static const std::string LOGGER_TAG = "Framebuffer";

    Framebuffer::Framebuffer(const FramebufferSpecification& s)
    : spec(s)
    {
        invalidate();
    }

    Framebuffer::~Framebuffer()
    {
        cleanup();
    }

    void Framebuffer::cleanup()
    {
        glDeleteFramebuffers(1, &renderID);
        glDeleteTextures(1, &colourAttachment);
        glDeleteTextures(1, &depthAttachement);
    }

    void Framebuffer::invalidate()
    {
        if (renderID) cleanup();

        glCreateFramebuffers(1, &renderID);
        glBindFramebuffer(GL_FRAMEBUFFER, renderID);

        glCreateTextures(GL_TEXTURE_2D, 1, &colourAttachment);
        glBindTexture(GL_TEXTURE_2D, colourAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachement);
        glBindTexture(GL_TEXTURE_2D, depthAttachement);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec.width, spec.height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachement, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            logError(LOGGER_TAG, "Framebuffer imcomplete");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::resize(uint32_t w, uint32_t h)
    {
        if (w == 0 || h == 0 ||w > maxFramebuffersize || h > maxFramebuffersize)
        {
            logWarn(LOGGER_TAG, "Framebuffer size incorrect! ", w, " ", h);
            return;
        }
        spec.width = w;
        spec.height = h;

        invalidate();
    }
}