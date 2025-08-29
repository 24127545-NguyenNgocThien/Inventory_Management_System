add_library(notifyLib STATIC
    notify.h
    notify.cpp
)

target_link_libraries(notifyLib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(notifyLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
