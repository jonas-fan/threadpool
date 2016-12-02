# threadpool
[![Build Status](https://travis-ci.org/yjfan/threadpool.svg?branch=master)](https://travis-ci.org/yjfan/threadpool)

The C++11 simple thread pool.

## Building
```sh
$ git clone https://github.com/yjfan/threadpool.git
$ cd threadpool/
$ mkdir build/
$ cd build/
$ cmake .. -DCMAKE_INSTALL_PREFIX=../bin/
$ make
```

## Installing
```sh
$ cd build/
$ make install
```
After executing `make install`, this library will be copied into `${CMAKE_INSTALL_PREFIX}/*`.

## Build our own application without CMake
To build our application, we should build threadpool with CMake. Otherwise we need to write makefiles by ourselves.
After building, the following command can help to build an application without CMake.
```sh
$ g++ example.cpp -Ithreadpool/bin/include -Lthreadpool/bin/lib -lthreadpool -lpthread -std=c++11
```
However, suggest that we should use CMake. It's a very powerful tool.

## Example
```cpp
#include <iostream>
#include "threadpool/threadpool.h"

static void routine(void *user_data)
{
    /* Do something ... */
    std::cout << "Hello threadpool!" << std::endl;
}

int main(int argc, char *argv[])
{
    /* Create a thread pool which has eight workers. */
    ThreadPool *pool = new ThreadPool(8);

    /* Push a routine into the pool. */
    pool->push(routine, NULL);

    /* Do something ... */

    /* Wait for all routines to be finished then destroy the pool. */
    delete pool;

    return 0;
}
```

## License
The MIT License (MIT)
