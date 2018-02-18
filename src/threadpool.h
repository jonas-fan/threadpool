#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>

class ThreadPool
{
public:
    ThreadPool(unsigned int size);
    ~ThreadPool();

    bool push(void (*routine)(void *), void *user_data);
    void join();

private:
    enum {
        RUNNING = 0,
        EXIT,
        EXIT_IMMEDIATELY
    };

    static void dispath(void *user_data);

private:
    int state_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::deque< std::function<void ()> > tasks_;
    std::deque< std::thread * > workers_;
};

#endif /* __THREAD_POOL_H__ */