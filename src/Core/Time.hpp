#pragma once
#include <chrono>

namespace EngineCore
{
    class Time
    {
    public:
        void tick()
        {
            auto now = std::chrono::high_resolution_clock::now();
            deltaTime = std::chrono::duration<float>(now - lastFrame).count();
            totalTime += deltaTime;
            lastFrame = now;
        }

        float getDeltaTime() const {return deltaTime;}
        float getTotalTime() const {return totalTime;}
        float getFPS() const {return 1.f/deltaTime;}

    private:
        std::chrono::high_resolution_clock::time_point lastFrame = std::chrono::high_resolution_clock::now();
        float deltaTime;
        float totalTime;
    };
}