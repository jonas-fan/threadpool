#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *    Simple thread pool
 *
 *    @date    2016/03/12
 *    @author  Jian <jianfan.tw@gmail.com>
 */

typedef struct threadpool_t threadpool_t;

typedef void (*threadpool_func)(void *);

threadpool_t * threadpool_create(unsigned int threads);
void threadpool_destroy(threadpool_t *pool);

unsigned int threadpool_threads(threadpool_t *pool);

int threadpool_run(threadpool_t *pool, threadpool_func function, void *user_data);

#ifdef __cplusplus
}
#endif

#endif  /*  THREAD_POOL_H_ */