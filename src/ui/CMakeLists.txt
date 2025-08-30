add_library(uiLib STATIC
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    productDialog.h
    productDialog.cpp
    selectionDialog.h
    selectionDialog.cpp
    invoiceDialog.h
    invoiceDialog.cpp
)

target_link_libraries(uiLib PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Charts
    commandManagerLib
    dataLib
    invoiceLib
    reportLib
    notifyLib
    commandTypesLib
)

target_include_directories(uiLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/commandManager
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/data
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/command/types
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/invoice
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/report
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/other
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/notifyLib
)
