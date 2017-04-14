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

    std::function<void ()> task_;
    std::recursive_mutex mutex_;
    std::condition_variable_any cond_var_;
    std::thread thread_;
};

#endif /* THREAD_WORKER_H_ */