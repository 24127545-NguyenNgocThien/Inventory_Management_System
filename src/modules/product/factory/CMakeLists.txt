add_library(productFactoryLib STATIC
    productfactory.h
    productfactory.cpp
)

target_link_libraries(productFactoryLib
    PUBLIC productTypesLib
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(productFactoryLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../types
)
