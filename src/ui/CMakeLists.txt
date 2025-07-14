add_library(ui STATIC
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    addproduct.h
    addproduct.cpp
    addproduct.ui
)

target_link_libraries(ui PRIVATE
    app
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../appController
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/product
    ${CMAKE_CURRENT_BINARY_DIR}
)
