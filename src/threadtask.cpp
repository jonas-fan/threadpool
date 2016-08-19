#include "threadtask.h"

ThreadTask::ThreadTask(void (*routine)(void *), void *user_data)
    : routine_(routine), user_data_(user_data)
{

}

ThreadTask::~ThreadTask()
{

}

void ThreadTask::run()
{
    if (this->routine_) {
        this->routine_(this->user_data_);
    }
}