add_library(productFactory STATIC
    productFactory.cpp
    productFactory.h
)

target_link_libraries(productFactory PRIVATE
    product
    Qt6::Core
)

target_include_directories(productFactory PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../product
)
