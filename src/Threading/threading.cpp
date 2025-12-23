#include "PCH.hpp"
#include "threading.hpp"

#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <list>
#include "tracy/Tracy.hpp"

#include "../Logging/Logger2.hpp"

namespace EngineCore
{
    static constexpr std::string_view LOGGER_TAG = "AsyncTask";

    static std::atomic_uint32_t threadIDCounter = 0;
    static std::unordered_map<uint32_t, uint32_t> threadIDMap;
    static std::mutex threadIDLock;

    static std::vector<std::thread> threads;
    static std::mutex threadpoolWorkLock;
    static std::condition_variable threadpoolWorkCondition;

    static bool shouldThreadsExit;
    static std::vector<ThreadpoolTask*> waitingTasks;
    static std::list<ThreadpoolTask*> readyTasks;

    void threadLoop()
    {
        ZoneScopedC(0x00FF77);
        while (true)
        {
            const bool didTask = executeNextTask();
            if (didTask) continue;

            std::unique_lock lock(threadpoolWorkLock);

            if (shouldThreadsExit) break;

            if(readyTasks.empty())
            {
                threadpoolWorkCondition.wait(lock);
            }

            lock.unlock();
        }
    }

    void updateWaitingTasks()
    {
        ZoneScoped;
        threadpoolWorkLock.lock();
        erase_if(waitingTasks, [](ThreadpoolTask* task)
        {
            const bool isReady = task->dependenciesCompleted();

            if (isReady) readyTasks.push_back(task);
            return isReady;
        });
        threadpoolWorkLock.unlock();
    }

    bool executeNextTask()
    {
        ThreadpoolTask* next = nullptr;
        threadpoolWorkLock.lock();
        if (!readyTasks.empty())
        {
            next = readyTasks.front();
            readyTasks.pop_front();
        }
        threadpoolWorkLock.unlock();

        if (next == nullptr)
        {
            return false;
        }

        next->tryExecuteTask();
        updateWaitingTasks();

        return true;
    }

    bool hasNextTask()
    {
        bool result = true;
        threadpoolWorkLock.lock();
        if (readyTasks.empty()) result = false;
        threadpoolWorkLock.unlock();
        return result;
    }

    void releaseThreadpool()
    {
        ZoneScoped;
        threadpoolWorkLock.lock();
        shouldThreadsExit = true;
        threadpoolWorkLock.unlock();

        threadpoolWorkCondition.notify_all();

        for (std::thread& t : threads)
        {
            t.join();
        }

        threads.clear();
    }

    void createThreadpool()
    {
        ZoneScoped;
        threadpoolWorkLock.lock();

        if (!threads.empty()) return;

        shouldThreadsExit = false;

        const int32_t availableCores = std::thread::hardware_concurrency() - 2;
        const uint32_t count = static_cast<uint32_t>(std::max(availableCores, 3));
        threads.reserve(count);
        for (uint32_t i = 0; i < count; i++)
        {
            threads.emplace_back([i]
            {
                char* threadName = new char[16];
                snprintf(threadName, 16, " Worker %i ", i);
                tracy::SetThreadName(threadName);
                threadLoop();
            });
        }

        threadpoolWorkLock.unlock();
    }

    uint32_t getCurrentThreadID()
    {
        threadIDLock.lock();
        uint32_t id = 0;
        const uint32_t hash = std::hash<std::thread::id> {}(std::this_thread::get_id());
        if (threadIDMap.contains(hash))
        {
            id = threadIDMap[hash];
        }
        else
        {
            id = threadIDCounter;
            threadIDCounter += 1;
            threadIDMap[hash] = id;
        }
        threadIDLock.unlock();
        return id;
    }

    void ThreadpoolTask::enqueue()
    {
        if (!stateReady()) return;

        threadpoolWorkLock.lock();

        const bool ready = dependenciesCompleted();

        if (ready) {readyTasks.push_back(this);}
        else {waitingTasks.push_back(this);}

        threadpoolWorkLock.unlock();
        threadpoolWorkCondition.notify_one();
    }

    bool ThreadpoolTask::completed() const
    {
        return state == ExecutionState::COMPLETED;
    }

    void ThreadpoolTask::awaitCompleted()
    {
        ZoneScoped;
        const bool executeImmiedate = tryExecuteTask();
        if (executeImmiedate) return;
        completionFuture.wait();
    }

    bool ThreadpoolTask::awaitTimeout(const uint32_t timeout) const
    {
        ZoneScoped;
        completionFuture.wait_for(std::chrono::milliseconds(timeout));
        return completed();
    }

    bool ThreadpoolTask::tryExecuteTask()
    {
        ZoneScoped;
        if (dependenciesCompleted() && stateReady())
        {
            state = ExecutionState::RUNNING;
            execute();
            completionPromise.set_value();
            state = ExecutionState::COMPLETED;
            return true;
        }

        return false;
    }

    bool ThreadpoolTask::dependenciesCompleted() const
    {
        for (const ThreadpoolTask* dep : dependsOn)
        {
            if (!dep->completed()) return false;
        }
        return true;
    }

    bool ThreadpoolTask::stateReady() const
    {
        return state == ExecutionState::WAITING;
    }
}