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
 *    @date    2015/10/08
 *    @author  Jian <jianfan.tw@gmail.com>
 */

typedef struct threadpool_t threadpool_t;

typedef void (*threadpool_func)(void *);

threadpool_t * threadpool_create(unsigned int threads_size);

void threadpool_destroy(threadpool_t *pool);

bool threadpool_run(threadpool_t *pool, threadpool_func function, void *data);

#ifdef __cplusplus
}
#endif

#endif  /*  SIMPLE_THREAD_POOL_H_ */