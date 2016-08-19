#ifndef THREAD_TASK_H_
#define THREAD_TASK_H_

class ThreadTask
{
public:
    ThreadTask(void (*routine)(void *), void *user_data);
    ~ThreadTask();

    void run();

private:
    void (*routine_)(void *);
    void *user_data_;
};

#endif /* THREAD_TASK_H_ */