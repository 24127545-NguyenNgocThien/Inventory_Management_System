add_subdirectory(types)
add_subdirectory(factory)

add_library(productLib STATIC
    product.h
    product.cpp
)

target_link_libraries(productLib PUBLIC
    productFactoryLib
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(productLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
