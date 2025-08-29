add_library(loggerLib STATIC
    log.cpp
    log.h
)

target_link_libraries(loggerLib PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(loggerLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
