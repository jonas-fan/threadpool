cmake_minimum_required(VERSION 2.8)

project(c-thread-pool-test C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O2")

include_directories(
    ${CMAKE_SOURCE_DIR}/include/
)

add_executable(test
    ./test.c
)

target_link_libraries(test
    cthreadpool
)