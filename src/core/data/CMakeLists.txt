add_library(dataLib STATIC
    data.h
    data.cpp
)

target_link_libraries(dataLib PUBLIC
    productLib
)

target_include_directories(dataLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
