add_library(loggerLib STATIC
    log.h
    log.cpp
)

target_include_directories(loggerLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
