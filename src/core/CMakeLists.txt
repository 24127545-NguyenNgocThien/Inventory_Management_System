add_subdirectory(database)
add_subdirectory(product)
add_subdirectory(productFactory)
add_subdirectory(command)
add_subdirectory(log)
add_subdirectory(notify)

add_library(core INTERFACE)

target_link_libraries(core INTERFACE
    product
    productFactory
    database
)
