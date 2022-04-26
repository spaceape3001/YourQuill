set(db_sources
    ./arg.cpp
    ./cdb.cpp
    ./file.cpp
    ./html.cpp
    ./workspace.cpp
)

add_library(yqdb SHARED ${db_sources})
target_link_libraries(yqdb PUBLIC yq)

#   override for temp
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/garbage)
add_executable( link_test_yqdb linktest.cpp )
target_link_libraries( link_test_yqdb yqdb )


