#include <thread_pool.h>

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

static void increment(void *data)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&mutex);

    unsigned int *number = (unsigned int *)data;

    ++(*number);

    pthread_mutex_unlock(&mutex);
}

int main(/*int argc, char *argv[]*/)
{
    threadpool_t *pool = threadpool_create(4);

    unsigned int count = 0;
    unsigned int index;

    for (index = 0; index < 1; ++index) {
        assert(threadpool_run(pool, increment, &count) == 0);
    }

    while (count != 1) {
        usleep(200 * 1000);
    }

    threadpool_destroy(pool);

    printf("[\033[32mOK\033[m]\n");

    return 0;
}