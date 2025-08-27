add_library(invoiceLib STATIC
    invoice.h
    invoice.cpp
    invoiceService.h
    invoiceService.cpp
)

target_include_directories(invoiceLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/other
    ${CMAKE_CURRENT_SOURCE_DIR}/../product
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/commandManager
    ${CMAKE_CURRENT_SOURCE_DIR}/../command
    ${CMAKE_CURRENT_SOURCE_DIR}/../command/types
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/data
)

target_link_libraries(invoiceLib PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    commandManagerLib
    dataLib
)
