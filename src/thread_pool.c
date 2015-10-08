#include <thread_pool.h>
#include <queue.h>

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct
{
    threadpool_func function;
    void *data;
} task_t;

struct threadpool_t
{
    pthread_t *threads;
    unsigned int threads_size;

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    queue_t *task_queue;

    bool release_flag;
};

static inline void * handler(void *data)
{
    threadpool_t *pool = (threadpool_t *)data;
    task_t task;
    int retval = -1;

    while (!pool->release_flag) {
        pthread_mutex_lock(&(pool->mutex));

        if (queue_size(pool->task_queue)) {
            retval = queue_pop(pool->task_queue, &task, sizeof(task_t));
        }
        else {
            if (pool->release_flag) {
                pthread_mutex_unlock(&(pool->mutex));
                break;
            }

            pthread_cond_wait(&(pool->cond), &(pool->mutex));

            pthread_mutex_unlock(&(pool->mutex));

            continue;
        }

        pthread_mutex_unlock(&(pool->mutex));

        if (!retval) {
            task.function(task.data);

            retval = -1;
        }
    }

    pthread_exit(NULL);
}

threadpool_t * threadpool_create(unsigned int threads_size)
{
    if (!threads_size) {
        return NULL;
    }

    threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));

    if (!pool) {
        return NULL;
    }

    memset(pool, 0, sizeof(threadpool_t));

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
    pthread_cond_init(&(pool->cond), NULL);

    for (unsigned int index = 0; index < pool->threads_size; ++index) {
        pthread_create(pool->threads + index, NULL, handler, pool);
    }

    return pool;
}

void threadpool_destroy(threadpool_t *pool)
{
    pool->release_flag = true;

    pthread_mutex_lock(&(pool->mutex));

    pthread_cond_broadcast(&(pool->cond));

    pthread_mutex_unlock(&(pool->mutex));

    for (unsigned int index = 0; index < pool->threads_size; ++index) {
        pthread_join(pool->threads[index], NULL);
    }

    free(pool->threads);

    pthread_cond_destroy(&(pool->cond));
    pthread_mutex_destroy(&(pool->mutex));

    while (queue_size(pool->task_queue)) {
        queue_pop(pool->task_queue, NULL, 0);
    }

    queue_destroy(pool->task_queue);

    free(pool);
}

bool threadpool_run(threadpool_t *pool, threadpool_func function, void *data)
{
    if (!function) {
        return false;
    }

    task_t task = {
        .function = function,
        .data = data
    };

    pthread_mutex_lock(&(pool->mutex));

    queue_push(pool->task_queue, &task, sizeof(task_t));

    pthread_cond_broadcast(&(pool->cond));

    pthread_mutex_unlock(&(pool->mutex));

    return true;
}