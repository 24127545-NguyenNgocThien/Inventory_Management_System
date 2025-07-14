add_library(product STATIC
    product.cpp
    product.h
)

target_link_libraries(product PRIVATE
    Qt6::Core
)
