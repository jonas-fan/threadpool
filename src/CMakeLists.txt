set(HEADERS ./threadpool.h)

set(SOURCES
    ./threadpool.cpp
    ./threadworker.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    pthread
)

install(FILES ${HEADERS}
    DESTINATION ${INSTALL_DIR}/include/threadpool/
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${INSTALL_DIR}/lib
)