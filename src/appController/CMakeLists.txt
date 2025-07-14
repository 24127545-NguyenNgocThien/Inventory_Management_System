add_library(app STATIC
    app.cpp
    app.h
)

target_link_libraries(app PRIVATE
    core
    Qt6::Core
)

target_include_directories(app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/product
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/productFactory
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/database
)

#####################################

add_executable(${PROJECT_NAME} WIN32
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    app
    ui
    Qt6::Widgets
    Qt6::Core
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/product
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
