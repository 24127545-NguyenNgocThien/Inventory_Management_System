add_subdirectory(command)
add_subdirectory(product)
add_subdirectory(invoice)
add_subdirectory(report)

add_library(modulesLib INTERFACE)

target_link_libraries(modulesLib INTERFACE
    productLib
    commandLib
    invoiceLib
    reportLib
)




