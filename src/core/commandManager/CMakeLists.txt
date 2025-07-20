add_library(commandManagerLib STATIC
    commandmanager.h
    commandmanager.cpp
)

target_link_libraries(commandManagerLib PUBLIC
    commandLib
    commandTypesLib
)

target_include_directories(commandManagerLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/command
)
