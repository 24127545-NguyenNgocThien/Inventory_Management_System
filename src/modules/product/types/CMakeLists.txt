add_library(productTypesLib STATIC
    electricproduct.h
    electricproduct.cpp

    waterproduct.h
    waterproduct.cpp

    otherproduct.h
    otherproduct.cpp
)

target_link_libraries(productTypesLib PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(productTypesLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
