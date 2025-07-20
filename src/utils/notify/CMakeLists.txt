add_library(notifyLib STATIC
    notify.h
    notify.cpp
)

target_include_directories(notifyLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
