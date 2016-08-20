#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>

class ThreadWorker;

class ThreadPool
{
public:
    ThreadPool(unsigned int size);
    ~ThreadPool();

    bool push(void (*routine)(void *), void *user_data);

private:
    static void dispath(void *user_data);

private:
    bool running_;
    unsigned int size_;

    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::thread dispatcher_;
    std::deque<ThreadWorker *> workers_;
    std::deque< std::function<void ()> > tasks_;
};

#endif /* THREAD_POOL_H_ */