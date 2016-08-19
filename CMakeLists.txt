cmake_minimum_required(VERSION 2.8)

project(threadpool C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -std=gnu99 -g")
set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

add_subdirectory(src)

# Test
add_executable(threads ./tests/threads.c)
target_link_libraries(threads ${PROJECT_NAME})