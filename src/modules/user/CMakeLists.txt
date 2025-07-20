add_library(userLib STATIC
    user.h
    user.cpp
)

target_include_directories(userLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

