#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include "SafeQueue.h"

class ThreadPool
{
private:
    std::vector<std::thread> workers_;
    SafeQueue<std::function<void()>> taskQueue_;
    std::atomic<bool> stop_;

public:
    explicit ThreadPool(size_t threadsCount) : stop_(false)
    {
        for (size_t i = 0; i < threadsCount; ++i)
        {

            workers_.emplace_back([this]()
                                  {
                while (!stop_) {
                    std::function<void()> task;
        
                    if (taskQueue_.pop(task)) {
                        task(); 
                    }
                } });
        }
    }

    template <typename F>
    void enqueue(F &&task)
    {
        taskQueue_.push(std::forward<F>(task));
    }

    ~ThreadPool()
    {
        stop_ = true;
        for (size_t i = 0; i < workers_.size(); ++i)
        {
            taskQueue_.push([]() {});
        }

        for (std::thread &worker : workers_)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }
};