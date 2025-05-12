
#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);    

    ~ThreadPool();

    void enqueue(std::function<void()> job);
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;

    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    void start(size_t numThreads);
    void shutdown();
};
