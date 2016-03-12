cmake_minimum_required(VERSION 2.8)

project(thread-pool-test C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -std=gnu99 -g")

include_directories(
    ${CMAKE_SOURCE_DIR}/include/
)

add_executable(test
    ./test.c
)

target_link_libraries(test
    ${TARGET_LIB}
)