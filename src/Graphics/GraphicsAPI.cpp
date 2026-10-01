#include "GraphicsAPI.hpp"

namespace EngineCore
{
    static GraphicsAPI activeAPI = GraphicsAPI::OpenGL;

    void setActiveGraphicsAPI(GraphicsAPI api)
    {
        activeAPI = api;
    }

    GraphicsAPI getActiveGraphicsAPI()
    {
        return activeAPI;
    }

    std::string getAPIString()
    {
        switch (activeAPI)
        {
            case GraphicsAPI::OpenGL: return "OpenGL";
            case GraphicsAPI::DirectX12: return "DX12";
            case GraphicsAPI::Vulkan: return "Vk";
            case GraphicsAPI::None: return "None?";
        }
    }
}