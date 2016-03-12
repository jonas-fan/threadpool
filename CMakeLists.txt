cmake_minimum_required(VERSION 2.8)

project(thread-pool C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -std=gnu99 -g")
set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

set(HEADER
    ./include/thread_pool.h
)

set(SOURCE
    ./src/thread_pool.c
)

set(TARGET_LIB threadpool)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${TARGET_LIB} STATIC ${SOURCE})

target_link_libraries(${TARGET_LIB} pthread)

install(FILES ${HEADER} DESTINATION ${INSTALL_DIR}/include)
install(TARGETS ${TARGET_LIB} DESTINATION ${INSTALL_DIR}/lib)

add_subdirectory(test)
