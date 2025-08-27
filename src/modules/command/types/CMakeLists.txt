add_library(commandTypesLib STATIC
    addcommand.h
    addcommand.cpp

    deletecommand.h
    deletecommand.cpp

    editcommand.h
    editcommand.cpp

    importCommand.h
    importCommand.cpp

    exportCommand.h
    exportCommand.cpp
)

target_link_libraries(commandTypesLib PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    productFactoryLib
)

target_include_directories(commandTypesLib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../product
    ${CMAKE_CURRENT_SOURCE_DIR}/../../product/factory
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../utils/other
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../core/data
    ${CMAKE_CURRENT_SOURCE_DIR}/../../invoice
)
