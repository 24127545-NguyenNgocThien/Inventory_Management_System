add_subdirectory(types)

add_library(commandLib STATIC
    command.h
)

target_link_libraries(commandLib PUBLIC
    commandTypesLib
)

target_include_directories(commandLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
