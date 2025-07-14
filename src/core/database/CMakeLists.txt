add_library(database STATIC
    database.cpp
    database.h
)

target_link_libraries(database PRIVATE
    Qt6::Core
)
