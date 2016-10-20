cmake_minimum_required(VERSION 2.8)

project(threadpool CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -pedantic -std=c++11 -g")

add_subdirectory(src)

# Tests
add_executable(threads
    ./tests/threads_test.cpp
)

target_link_libraries(threads
    ${PROJECT_NAME}
)