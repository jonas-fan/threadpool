#include "threadworker.h"

ThreadWorker::ThreadWorker()
    : running_(true), idle_(true), task_(NULL), mutex_(), cond_var_()
{
    this->thread_ = new std::thread(ThreadWorker::routine, this);
}

ThreadWorker::~ThreadWorker()
{
    this->mutex_.lock();

    this->running_ = false;
    this->cond_var_.notify_one();

    this->mutex_.unlock();

    this->thread_->join();
    delete this->thread_;

    if (this->task_) {
        delete this->task_;
    }
}

bool ThreadWorker::isIdle()
{
    std::unique_lock<std::mutex> lock(this->mutex_);

    return this->idle_;
}

bool ThreadWorker::assign(ThreadTask *task)
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

    while (true) {
        lock.lock();

        if (!self->running_) {
            return;
        }

        while (!self->task_) {
            self->idle_ = true;
            self->cond_var_.wait(lock);

            if (!self->running_) {
                return;
            }
        }

        ThreadTask *task = self->task_;

        self->task_ = NULL;

        lock.unlock();

        if (task) {
            task->run();
            delete task;
        }
    }
}