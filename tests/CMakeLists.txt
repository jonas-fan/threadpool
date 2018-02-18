include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_executable(threads_test
    ./threads_test.cpp
)

target_link_libraries(threads_test
    ${PROJECT_NAME}
)