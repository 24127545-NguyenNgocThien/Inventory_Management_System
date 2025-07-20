add_library(uiLib STATIC
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(uiLib PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    commandManagerLib
    dataLib
)

target_include_directories(uiLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/commandManager
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/data
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/command/types
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/other
)
