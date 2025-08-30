add_subdirectory(core)
add_subdirectory(modules)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(${PROJECT_NAME}
    main.cpp
#    ${CMAKE_SOURCE_DIR}/assets/.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    appLib
    uiLib
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Charts
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    WIN32_EXECUTABLE ON
)
