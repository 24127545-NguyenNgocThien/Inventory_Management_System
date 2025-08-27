add_library(reportLib STATIC
    report.h
    report.cpp
)

target_include_directories(reportLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../invoice
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/data
)

target_link_libraries(reportLib PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    dataLib
    invoiceLib
)
