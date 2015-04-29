#include <thread_pool.h>
#include <queue.h>

#include <stdlib.h>
#include <string.h>
#include <pthread.h>


/**
 *    Private
 */

typedef struct task_t
{
    ThreadPoolFunction function;
    void *data;
} Task;

struct thread_pool_t
{
    pthread_t *threads;
    unsigned int threads_size;

    Queue *task_queue;

    pthread_mutex_t mutex;

    bool release_flag;
};

static void * handler(void *data);


/**
 *    Public
 */

ThreadPool * threadpool_create(unsigned int threads_size)
{
    if (!threads_size) {
        return NULL;
    }

    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));

    if (!pool) {
        return NULL;
    }

    memset(pool, 0, sizeof(ThreadPool));

    pool->task_queue = queue_create();

    if (!pool->task_queue) {
        free(pool);

        return NULL;
    }

    pool->release_flag = false;
    pool->threads_size = threads_size;
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * pool->threads_size);

    if (!pool->threads) {
        queue_destroy(pool->task_queue);

        free(pool);

        return NULL;
    }

    pthread_mutex_init(&(pool->mutex), NULL);

    unsigned int index;

    for (index = 0; index < pool->threads_size; ++index) {
        pthread_create(pool->threads + index, NULL, handler, pool);
    }

    return pool;
}

void threadpool_destroy(ThreadPool *pool)
{
    pool->release_flag = true;

    unsigned int index;

    for (index = 0; index < pool->threads_size; ++index) {
        pthread_join(pool->threads[index], NULL);
    }

    free(pool->threads);

    pthread_mutex_destroy(&(pool->mutex));

    while (queue_size(pool->task_queue)) {
        Task *task = (Task *)queue_front(pool->task_queue);

        queue_pop(pool->task_queue);

        free(task);
    }

    queue_destroy(pool->task_queue);

    free(pool);
}

bool threadpool_add(ThreadPool *pool, ThreadPoolFunction function, void *data)
{
    if (!function) {
        return false;
    }

    Task *task = (Task *)malloc(sizeof(Task));

    if (!task) {
        return false;
    }

    task->function = function;
    task->data = data;

    pthread_mutex_lock(&(pool->mutex));

    queue_push(pool->task_queue, (void *)task);

    pthread_mutex_unlock(&(pool->mutex));

    return true;
}


/**
 *    Private
 */

static void * handler(void *data)
{
    ThreadPool *pool = (ThreadPool *)data;
    Task *task = NULL;

    while (!pool->release_flag) {
        pthread_mutex_lock(&(pool->mutex));

        if (queue_size(pool->task_queue)) {
            task = (Task *)queue_front(pool->task_queue);

            queue_pop(pool->task_queue);
        }

        pthread_mutex_unlock(&(pool->mutex));

        if (task) {
            task->function(task->data);

            free(task);

            task = NULL;
        }
    }

    pthread_exit(NULL);
}