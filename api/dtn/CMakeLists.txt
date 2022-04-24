set(dtn_sources
    ./dtn.cpp
)

add_library(yqdtn SHARED ${dtn_sources})
target_link_libraries(yqdtn PUBLIC yq yqdb)

