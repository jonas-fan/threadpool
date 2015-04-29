cmake_minimum_required(VERSION 2.8)

project(c-thread-pool C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O2")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/simple-container/include/
)

add_library(cthreadpool STATIC
    ./src/cthreadpool.c
)

target_link_libraries(cthreadpool
    pthread
    simplecontainer
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/simple-container)