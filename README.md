# thread-pool
[![Build Status](https://travis-ci.org/yjfan/thread-pool.svg?branch=master)](https://travis-ci.org/yjfan/thread-pool)

Simple thread pool implementation in C

## Build
```
$ git clone https://github.com/yjfan/thread-pool.git
$ cd thread-pool/
$ makdir build/
$ cd build/
$ cmake ..
$ make -j4
$ make install
```

## Example
```cpp
void worker(void *user_data)
{
    // do something ...
}
```

```cpp
threadpool_t *pool = threadpool_create(8);

threadpool_run(pool, worker, NULL);

// do something ...

threadpool_destroy(pool);
```

## License
The MIT License (MIT)
