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
}