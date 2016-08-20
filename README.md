# threadpool
[![Build Status](https://travis-ci.org/yjfan/threadpool.svg?branch=master)](https://travis-ci.org/yjfan/threadpool)

The C++11 simple thread pool.

## Building
```sh
$ git clone https://github.com/yjfan/threadpool.git
$ cd threadpool/
$ mkdir build/
$ cd build/
$ cmake ..
$ make
```

## Installing
```sh
$ cd build/
$ make install
```
After executing `make install`, this library will be copied into `threadpool/bin/*`.

## Example
```cpp
#include "threadpool/threadpool.h"

static void worker(void *user_data)
{
    /* Do something ... */
}

int main(int argc, char *argv[])
{
    /* Create a thread pool which has eight workers. */
    ThreadPool *pool = new ThreadPool(8);

    /* Push a routine into the pool. */
    pool->push(worker, NULL)

    /* Do something ... */

    /* Wait for all routines to be finished then destroy the pool. */
    delete pool;

    return 0;
}
```

## License
The MIT License (MIT)
