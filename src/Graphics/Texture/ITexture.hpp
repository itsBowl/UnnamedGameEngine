#pragma once
#include <string>
#include <cstdint>
#include "Errors.hpp"

namespace EngineCore
{
    class ITexture
    {
    public:
        virtual ~ITexture() = default;

        virtual int load(const std::string& p) = 0;
        virtual void bind(uint32_t s = 0) const = 0;
        virtual void unbind() = 0;

        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
        virtual bool exists() = 0;
        virtual const std::string& getName() const = 0;
    };
}