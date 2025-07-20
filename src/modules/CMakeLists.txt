add_subdirectory(command)
add_subdirectory(product)
add_subdirectory(user)

add_library(modulesLib INTERFACE)

target_link_libraries(modulesLib INTERFACE
    productLib
    commandLib
    userLib
)




