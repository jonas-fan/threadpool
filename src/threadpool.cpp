#include "threadpool.h"
#include "threadworker.h"

template <typename Iterator>
static bool assignTask(std::function<void ()> &task, Iterator begin, Iterator end)
{
    while (begin != end) {
        ThreadWorker *worker = *begin;

        if (worker->isIdle()) {
            worker->assign(task);
            return true;
        }

        ++begin;
    }

    return false;
}

ThreadPool::ThreadPool(unsigned int size)
    : running_(true), size_(size), mutex_(), cond_var_(),
      workers_(size), tasks_()
{
    for (auto iter = this->workers_.begin(); iter != this->workers_.end(); ++iter) {
        *iter = new ThreadWorker();
    }

    this->dispatcher_ = std::thread(ThreadPool::dispath, this);
}

ThreadPool::~ThreadPool()
{
    this->mutex_.lock();

    this->running_ = false;
    this->cond_var_.notify_one();

    this->mutex_.unlock();

    this->dispatcher_.join();

    for (ThreadWorker *worker : this->workers_) {
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
    std::function<void ()> task = NULL;

    while (true) {
        lock.lock();

        if (self->tasks_.empty() && !self->running_) {
            return;
        }

        while (self->tasks_.empty()) {
            self->cond_var_.wait(lock);

            if (!self->tasks_.empty()) {
                break;
            }
            else if (!self->running_) {
                return;
            }
        }

        task = self->tasks_.front();

        self->tasks_.pop_front();

        lock.unlock();

        if (!task) {
            continue;
        }

        while (!assignTask(task, self->workers_.begin(), self->workers_.end())) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }
}