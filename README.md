# threadpool

[![Build Status](https://travis-ci.org/jonas-fan/threadpool.svg?branch=master)](https://travis-ci.org/jonas-fan/threadpool)

A thread pool implementation in C++.

## Building

```sh
$ git clone https://github.com/jonas-fan/threadpool.git
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

## Example

```cpp
#include <iostream>
#include "threadpool.h"

static void routine(void *user_data)
{
    std::cout << "Hello threadpool!" << std::endl;
}

int main(int argc, char *argv[])
{
    /* Create a thread pool with eight workers */
    ThreadPool pool(8);

    /* Assign a task */
    pool.push(routine, NULL);

    /* Do something ... */

    /* Wait for all routines to be finished */
    pool.join();

    return 0;
}
```

## License

The MIT License (MIT)