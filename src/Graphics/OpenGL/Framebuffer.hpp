#pragma once
#include "PCH.hpp"
#include "gl/gl3w.h"

namespace EngineCore
{


    struct FramebufferSpecification
    {
        uint32_t width, height;
        uint32_t samples = 1;
        bool swapchainTarget = false;
    };

    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferSpecification&);
        ~Framebuffer();

        void bind();
        void unbind();

        void invalidate();
        void cleanup();

        void resize(uint32_t width, uint32_t height);

        inline uint32_t getColourAttachmentID() const { return colourAttachment; }
        inline const FramebufferSpecification& getSpec() const { return spec; }

    private:
        uint32_t renderID;
        uint32_t colourAttachment, depthAttachement;

        FramebufferSpecification spec;
    };

    


}