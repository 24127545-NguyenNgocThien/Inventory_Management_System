add_subdirectory(types)

add_library(commandLib STATIC
    command.h
)

target_link_libraries(commandLib PUBLIC
    commandTypesLib
    loggerLib
)

target_include_directories(commandLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../invoice
    ${CMAKE_CURRENT_SOURCE_DIR}/../../loggerLib
)
