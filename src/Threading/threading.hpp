#pragma once
#include <atomic>
#include <condition_variable>
#include <future>

#include "../Common/sized_numerics.hpp"

namespace EngineCore
{
    void createThreadpool();
    void releaseThreadpool();
    
    bool executeNextTask();
    bool hasNextTask();

    uint32_t getCurrentThreadID();

    class ThreadpoolTask
    {
        public:
        ThreadpoolTask(const std::initializer_list<ThreadpoolTask*> deps) : dependsOn(deps)
        {
            completionPromise = std::promise<void>();
            completionFuture = completionPromise.get_future();
        }

        ThreadpoolTask() : ThreadpoolTask({}) {}
        virtual ~ThreadpoolTask() = default;

        void enqueue();
        bool completed() const;
        void awaitCompleted();
        bool awaitTimeout(u32) const;

        protected:
        virtual void execute() = 0;

        private:
        enum class ExecutionState : u8{
            WAITING,
            RUNNING,
            COMPLETED
        };

        bool tryExecuteTask();

        bool dependenciesCompleted() const;

        bool stateReady() const;

        std::atomic<ExecutionState> state = ExecutionState::WAITING;

        std::vector<ThreadpoolTask*> dependsOn;

        std::promise<void> completionPromise;
        std::future<void> completionFuture;

        friend bool executeNextTask();
        friend void updateWaitingTasks();


    };
}