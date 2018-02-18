set(HEADERS ${CMAKE_CURRENT_SOURCE}threadpool.h)
set(SOURCES ${CMAKE_CURRENT_SOURCE}threadpool.cpp)

# static
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    pthread
)

# Install
install(FILES ${HEADERS}
    DESTINATION include/threadpool/
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION lib
)