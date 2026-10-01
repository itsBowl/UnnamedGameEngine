#pragma once
#include <string>

namespace EngineCore
{
    enum class GraphicsAPI
    {
        None,
        OpenGL,
        Vulkan,
        DirectX12
    };

    void setActiveGraphicsAPI(GraphicsAPI api);
    GraphicsAPI getActiveGraphicsAPI();
    std::string getAPIString();
}