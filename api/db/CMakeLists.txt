set(db_sources
    ./arg.cpp
    ./cdb.cpp
    ./file.cpp
    ./html.cpp
    ./workspace.cpp
)

add_library(yqdb SHARED ${db_sources})
target_link_libraries(yqdb PUBLIC yq)

