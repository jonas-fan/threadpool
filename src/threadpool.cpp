#include "threadpool.h"
#include "threadworker.h"
#include <chrono>

// template <typename Iterator>
// static inline ThreadWorker * get_idle_worker(Iterator first, Iterator last)
// {
//     while (first != last) {
//         if ((*first)->isIdle()) {
//             return *first;
//         }

//         ++first;
//     }

//     return NULL;
// }

ThreadPool::ThreadPool(unsigned int size)
    : running_(true), size_(size), mutex_(), cond_var_(),
      tasks_(), workers_(size)
{
    for (std::deque<ThreadWorker *>::iterator iter = this->workers_.begin();
         iter != this->workers_.end();
         ++iter)
    {
        *iter = new ThreadWorker();
    }

    this->dispatcher_ = new std::thread(ThreadPool::dispath, this);
}

ThreadPool::~ThreadPool()
{
    while (!this->tasks_.empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    this->mutex_.lock();

    this->running_ = false;
    this->cond_var_.notify_one();

    this->mutex_.unlock();

    this->dispatcher_->join();
    delete this->dispatcher_;

    for (ThreadWorker *worker : this->workers_) {
        delete worker;
    }

    for (ThreadTask *task : this->tasks_) {
        delete task;
    }
}

bool ThreadPool::push(void (*routine)(void *), void *user_data)
{
    if (!routine) {
        return false;
    }

    ThreadTask *task = new ThreadTask(routine, user_data);

    std::unique_lock<std::mutex> lock(this->mutex_);

    if (!this->running_) {
        return false;
    }

    this->tasks_.push_back(task);
    this->cond_var_.notify_one();

    return true;
}

void ThreadPool::dispath(void *user_data)
{
    ThreadPool *self = reinterpret_cast<ThreadPool *>(user_data);

    std::unique_lock<std::mutex> lock(self->mutex_, std::defer_lock);

    while (true) {
        lock.lock();

        if (!self->running_) {
            return;
        }

        while (self->tasks_.empty()) {
            self->cond_var_.wait(lock);

            if (!self->running_) {
                return;
            }
        }

        ThreadTask *task = self->tasks_.front();

        self->tasks_.pop_front();

        lock.unlock();

        if (!task) {
            continue;
        }

        bool assign = false;

        while (!assign) {
            for (ThreadWorker *worker : self->workers_) {
                if (worker->isIdle()) {
                    worker->assign(task);
                    assign = true;
                    break;
                }
            }
        }

        // ThreadWorker *worker = get_idle_worker(self->workers_.begin(), self->workers_.end());

        // if (worker) {
        //     worker->assign(task);
        // }
        // else {
        //     // TODO: adjust
        //     delete task;
        // }
    }
}