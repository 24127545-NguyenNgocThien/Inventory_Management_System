add_library(dataLib STATIC
    data.h
    data.cpp
)

target_link_libraries(dataLib PUBLIC
    productLib
    productFactoryLib
    PRIVATE invoiceLib
    loggerLib
    notifyLib
)

target_include_directories(dataLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/invoice
    ${CMAKE_CURRENT_SOURCE_DIR}/../../loggerLib
    ${CMAKE_CURRENT_SOURCE_DIR}/../../notifyLib
)
