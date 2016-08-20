#ifndef THREAD_WORKER_H_
#define THREAD_WORKER_H_

#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

class ThreadWorker
{
public:
    ThreadWorker();
    ~ThreadWorker();

    bool isIdle();
    bool assign(std::function<void ()> task);

private:
    static void routine(void *user_data);

private:
    bool running_;
    bool idle_;

    std::function<void ()> task_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::thread thread_;
};

#endif /* THREAD_WORKER_H_ */