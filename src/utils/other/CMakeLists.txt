add_library(otherLib STATIC
    otherutils.h
)

target_link_libraries(otherLib PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    productLib
)

target_include_directories(otherLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/product
)
