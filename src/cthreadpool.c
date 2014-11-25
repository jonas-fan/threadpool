#include <cthreadpool.h>
#include <queue.h>

#include <stdlib.h>
#include <pthread.h>


/**
 *    Private
 */

typedef struct c_task_t
{
    void (*function)(void *);
    void *data;
} CTask;

struct c_thread_pool_t
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

CThreadPool * cthreadpool_create(unsigned int threads)
{
    CThreadPool *pool = (CThreadPool *)malloc(sizeof(CThreadPool));

    if (!pool) {
        return NULL;
    }

    pool->release_flag = false;

    pool->task_queue = queue_create();

    if (!pool->task_queue) {
        free(pool);

        return NULL;
    }

    pthread_mutex_init(&(pool->mutex), NULL);

    pool->threads_size = threads;
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * pool->threads_size);

    if (!pool->threads) {
        pthread_mutex_destroy(&(pool->mutex));

        queue_destroy(pool->task_queue);

        free(pool);

        return NULL;
    }

    unsigned int index;

    for (index = 0; index < pool->threads_size; ++index) {
        pthread_create(pool->threads + index, NULL, handler, pool);
    }

    return pool;
}

void cthreadpool_destroy(CThreadPool *pool)
{
    pool->release_flag = true;

    unsigned int index;

    for (index = 0; index < pool->threads_size; ++index) {
        pthread_join(pool->threads[index], NULL);
    }

    free(pool->threads);

    pthread_mutex_destroy(&(pool->mutex));

    while (queue_size(pool->task_queue)) {
        CTask *task = (CTask *)queue_front(pool->task_queue);

        queue_pop(pool->task_queue);

        free(task);
    }

    queue_destroy(pool->task_queue);

    free(pool);
}

bool cthreadpool_add(CThreadPool *pool,
                     void (*function)(void *),
                     void *data)
{
    CTask *task = (CTask *)malloc(sizeof(CTask));

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
    CThreadPool *pool = (CThreadPool *)data;
    CTask *task = NULL;

    while (!pool->release_flag) {
        pthread_mutex_lock(&(pool->mutex));

        if (queue_size(pool->task_queue)) {
            task = (CTask *)queue_front(pool->task_queue);

            queue_pop(pool->task_queue);
        }

        pthread_mutex_unlock(&(pool->mutex));

        if (task &&
            task->function)
        {
            task->function(task->data);
        }

        free(task);

        task = NULL;
    }

    pthread_exit(NULL);
}