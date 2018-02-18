#include "threadpool.h"

ThreadPool::ThreadPool(unsigned int size)
    : state_(ThreadPool::RUNNING), mutex_(), cond_var_(), tasks_(), workers_()
{
    for (unsigned int index = 0; index < size; ++index) {
        this->workers_.push_back(new std::thread(ThreadPool::dispath, this));
    }
}

ThreadPool::~ThreadPool()
{
    this->mutex_.lock();
    this->state_ = ThreadPool::EXIT_IMMEDIATELY;
    this->cond_var_.notify_all();
    this->mutex_.unlock();

    for (std::thread *worker : this->workers_) {
        worker->join();
        delete worker;
    }
}

bool ThreadPool::push(void (*routine)(void *), void *user_data)
{
    if (!routine) {
        return false;
    }

    std::function<void ()> task = std::bind(routine, user_data);

    std::unique_lock<std::mutex> lock(this->mutex_);

    if (this->state_ != ThreadPool::RUNNING) {
        return false;
    }

    this->tasks_.push_back(task);
    this->cond_var_.notify_one();

    return true;
}

void ThreadPool::join()
{
    {
        std::unique_lock<std::mutex> lock(this->mutex_);

        if (this->state_ != ThreadPool::RUNNING) {
            return;
        }

        this->state_ = ThreadPool::EXIT;
        this->cond_var_.notify_all();
    }

    for (std::thread *worker : this->workers_) {
        worker->join();
        delete worker;
    }

    this->workers_.clear();
}

bool ThreadPool::shouldExit() const
{
    if (this->state_ == ThreadPool::EXIT_IMMEDIATELY) {
        return true;
    }

    if ((this->state_ == ThreadPool::EXIT) && this->tasks_.empty()) {
        return true;
    }

    return false;
}

void ThreadPool::dispath(void *user_data)
{
    ThreadPool *self = reinterpret_cast<ThreadPool *>(user_data);

    std::unique_lock<std::mutex> lock(self->mutex_, std::defer_lock);
    std::function<void ()> task = NULL;

    while (true) {
        lock.lock();

        if (self->shouldExit()) {
            return;
        }

        while (self->tasks_.empty()) {
            self->cond_var_.wait(lock);

            if (self->shouldExit()) {
                return;
            }
        }

        task = self->tasks_.front();

        self->tasks_.pop_front();

        lock.unlock();

        if (task) {
            task();
        }
    }
}