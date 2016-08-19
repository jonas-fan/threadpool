# threadpool
[![Build Status](https://travis-ci.org/yjfan/threadpool.svg?branch=master)](https://travis-ci.org/yjfan/threadpool)

Simple thread pool implementation in C

## Build
```
$ git clone https://github.com/yjfan/threadpool.git
$ cd threadpool/
$ mkdir build/
$ cd build/
$ cmake ..
$ make
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
