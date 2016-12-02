#include "threadworker.h"

ThreadWorker::ThreadWorker()
    : running_(true), idle_(true), task_(NULL),
      mutex_(), cond_var_(), thread_(ThreadWorker::routine, this)
{

}

ThreadWorker::~ThreadWorker()
{
    this->mutex_.lock();

    this->running_ = false;
    this->cond_var_.notify_one();

    this->mutex_.unlock();

    this->thread_.join();
}

bool ThreadWorker::isIdle()
{
    std::unique_lock<std::mutex> lock(this->mutex_);

    return this->idle_;
}

bool ThreadWorker::assign(std::function<void ()> task)
{
    if (!task) {
        return false;
    }

    std::unique_lock<std::mutex> lock(this->mutex_);

    if (!this->running_ || !this->idle_) {
        return false;
    }

    this->idle_ = false;
    this->task_ = task;
    this->cond_var_.notify_one();

    return true;
}

void ThreadWorker::routine(void *user_data)
{
    ThreadWorker *self = reinterpret_cast<ThreadWorker *>(user_data);

    std::unique_lock<std::mutex> lock(self->mutex_, std::defer_lock);
    std::function<void ()> task = NULL;

    while (true) {
        lock.lock();

        if (!self->task_ && !self->running_) {
            return;
        }

        while (!self->task_) {
            self->idle_ = true;
            self->cond_var_.wait(lock);

            if (self->task_) {
                break;
            }
            else if (!self->running_) {
                return;
            }
        }

        std::swap(task, self->task_);

        lock.unlock();

        if (task) {
            task();
            task = NULL;
        }
    }
}