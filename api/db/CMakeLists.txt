set(db_sources
    ./_arg.cpp
    ./_cdb.cpp
    ./_file.cpp
    ./_html.cpp
)

add_library(yqdb SHARED ${db_sources})
target_link_libraries(yqdb PUBLIC yq)

