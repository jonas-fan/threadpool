#ifndef SIMPLE_THREAD_POOL_H_
#define SIMPLE_THREAD_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 *    Reference:
 *        https://github.com/mbrossard/threadpool
 */

/**
 *    Simple thread pool
 *
 *    @verison 0.2.0
 *    @date    2015/04/29
 *    @author  Jian <jianfan.tw@gmail.com>
 */

typedef struct thread_pool_t ThreadPool;

typedef void (*ThreadPoolFunction)(void *);

ThreadPool * threadpool_create(unsigned int threads_size);

void threadpool_destroy(ThreadPool *pool);

bool threadpool_add(ThreadPool *pool, ThreadPoolFunction function, void *data);

#ifdef __cplusplus
}
#endif

#endif  /*  SIMPLE_THREAD_POOL_H_ */