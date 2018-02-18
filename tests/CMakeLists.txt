include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_executable(test_threadpool
    ./test_threadpool.cpp
)

target_link_libraries(test_threadpool
    ${PROJECT_NAME}
)