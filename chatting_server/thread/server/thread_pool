#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) { // stop 초기화를 하면서 ThreadPool 생성
        start(numThreads);
    }

    ~ThreadPool() {
        shutdown();
    }

    void enqueue(std::function<void()> job) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            jobs.push(job);
        }
        condition.notify_one();  // worker 하나 깨움
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;

    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    void start(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> job;

                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this]() { return stop || !jobs.empty(); });

                        if (stop && jobs.empty())
                            return;

                        job = std::move(jobs.front());
                        jobs.pop();
                    }

                    job();  // 작업 실행
                }
            });
        }
    }

    void shutdown() {
        stop = true;
        condition.notify_all();

        for (std::thread &worker : workers) {
            if (worker.joinable())
                worker.join();
        }
    }
};
