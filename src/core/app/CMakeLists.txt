add_library(appLib STATIC
    app.h
    app.cpp
)

target_link_libraries(appLib PUBLIC
    dataLib
    commandManagerLib
    uiLib
    modulesLib
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_include_directories(appLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ui
)
