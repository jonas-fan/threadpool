#include <cthreadpool.h>

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

void increment(void *data)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&mutex);

    int *number = (int *)data;

    ++(*number);

    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
    CThreadPool *pool = cthreadpool_create(4);

    int count = 0;
    unsigned int index;

    for (index = 0; index < 100; ++index) {
        assert(cthreadpool_add(pool, increment, &count));
    }

    sleep(5);

    assert(count == 100);

    cthreadpool_destroy(pool);

    printf("[\033[32mOK\033[m]\n");

    return 0;
}