#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue
{
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

public:
    SafeQueue() = default;
    ~SafeQueue() = default;

    // Добавление задачи в очередь
    void push(T item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(std::move(item));
        cv_.notify_one();
    }

    bool pop(T &item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]()
                 { return !queue_.empty(); });

        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.empty();
    }
};