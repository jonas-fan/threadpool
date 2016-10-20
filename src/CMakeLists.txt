set(HEADERS ./threadpool.h)

set(SOURCES
    ./threadpool.cpp
    ./threadworker.cpp
)

# static
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    pthread
)

# shared
#set(TARGET_LIB_SHARED "${PROJECT_NAME}_shared")
#add_library(${TARGET_LIB_SHARED} SHARED ${SOURCES})

#target_link_libraries(${TARGET_LIB_SHARED}
#    pthread
#)

#set_target_properties(${TARGET_LIB_SHARED}
#    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
#)

# Install
install(FILES ${HEADERS}
    DESTINATION include/threadpool/
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

#install(TARGETS ${TARGET_LIB_SHARED}
#    DESTINATION ${INSTALL_DIR}/lib
#)