#include <thread_pool.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

typedef struct threadpool_task_t threadpool_task_t;

struct threadpool_task_t
{
    threadpool_func function;
    void *user_data;

    threadpool_task_t *previous;
    threadpool_task_t *next;
};

struct threadpool_t
{
    bool running;
    pthread_t *threads;
    unsigned int threads_size;

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    threadpool_task_t *queue_head;
    threadpool_task_t *queue_tail;
    unsigned int queue_size;
};

static inline threadpool_task_t * task_create(threadpool_func function, void *user_data)
{
    threadpool_task_t *task = (threadpool_task_t *)malloc(sizeof(threadpool_task_t));

    if (task) {
        memset(task, 0, sizeof(threadpool_task_t));

        task->function = function;
        task->user_data = user_data;
    }

    return task;
}

static inline void task_destroy(threadpool_task_t *task)
{
    free(task);
}

static inline void * handler(void *user_data)
{
    threadpool_t *pool = (threadpool_t *)user_data;

    while (pool->running) {
        threadpool_task_t *task = NULL;

        pthread_mutex_lock(&(pool->mutex));

        if (pool->queue_size) {
            if (pool->queue_head == pool->queue_tail) {
                task = pool->queue_tail;

                pool->queue_head = NULL;
                pool->queue_tail = NULL;
            }
            else {
                task = pool->queue_tail;
                task->previous->next = NULL;

                pool->queue_tail = task->previous;
            }

            --pool->queue_size;
        }
        else {
            if (!pool->running) {
                pthread_mutex_unlock(&(pool->mutex));
                break;
            }

            pthread_cond_wait(&(pool->cond), &(pool->mutex));
            pthread_mutex_unlock(&(pool->mutex));

            continue;
        }

        pthread_mutex_unlock(&(pool->mutex));

        task->function(task->user_data);

        task_destroy(task);
    }

    pthread_exit(NULL);
}

threadpool_t * threadpool_create(unsigned int threads)
{
    if (!threads) {
        return NULL;
    }

    threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));

    if (!pool) {
        return NULL;
    }

    memset(pool, 0, sizeof(threadpool_t));

    pool->running = true;
    pool->threads_size = threads;
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * pool->threads_size);

    if (!pool->threads) {
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
    pool->running = false;

    pthread_mutex_lock(&(pool->mutex));
    pthread_cond_broadcast(&(pool->cond));
    pthread_mutex_unlock(&(pool->mutex));

    for (unsigned int index = 0; index < pool->threads_size; ++index) {
        pthread_join(pool->threads[index], NULL);
    }

    pthread_cond_destroy(&(pool->cond));
    pthread_mutex_destroy(&(pool->mutex));

    while (pool->queue_size) {
        threadpool_task_t *task = pool->queue_head;

        pool->queue_head = task->next;

        --pool->queue_size;

        task_destroy(task);
    }

    free(pool->threads);
    free(pool);
}

unsigned int threadpool_threads(threadpool_t *pool)
{
    return pool->threads_size;
}

int threadpool_run(threadpool_t *pool, threadpool_func function, void *user_data)
{
    if (!function) {
        return -1;
    }

    threadpool_task_t *task = task_create(function, user_data);

    if (!task) {
        return -1;
    }

    pthread_mutex_lock(&(pool->mutex));

    if (pool->queue_size) {
        task->next = pool->queue_head;
        task->next->previous = task;

        pool->queue_head = task;
    }
    else {
        pool->queue_head = task;
        pool->queue_tail = task;
    }

    ++pool->queue_size;

    pthread_cond_broadcast(&(pool->cond));

    pthread_mutex_unlock(&(pool->mutex));

    return 0;
}