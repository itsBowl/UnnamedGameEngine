#include "PCH.hpp"
#include "Render3d.hpp"

namespace EngineCore
{
    static const std::string LOGGER_TAG = "Render3D";
    void Render3d::preprocessing()
    {
        //this would contain any things we want to call that might not be handled by the render system
        //could be setting clearcolour but the render system shoudl handle that itself
    }

    void Render3d::setupTri()
    {
        BufferLayout layout{
            BufferElement(ShaderDataType::Float3, "a_Position"),
            BufferElement(ShaderDataType::Float3, "a_VertexColour"),
        };
        triArray.create();
        glm::vec3 verts [6] = 
        {
            glm::vec3(-.5f, -.5f, .0f), glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(.5f, -.5f, .0f), glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(.0f, .5f, .0f), glm::vec3(0.f, 0.f, 1.f),
        };
        triBuffer.create(verts, sizeof(verts));
        uint32_t idx[3] = {0, 1, 2};
        triIndex.create(idx, sizeof(idx) / sizeof(uint32_t), GL_STATIC_DRAW);

        triArray.bind();
        triArray.addVertexBuffer(triBuffer, layout);
        triArray.setIndexBuffer(triIndex);

        
    }

    VertexArray& Render3d::getTri()
    {
        logInfo(LOGGER_TAG, "Rendering triangle");
        return triArray;
    }

    void Render3d::renderTri()
    {
        glClearColor(1.f, .1f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.bind();
        triArray.bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }
}