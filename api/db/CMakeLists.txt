set(db_sources
    ./atomsys.cpp
    ./db.cpp
    ./docsys.cpp
    ./filesys.cpp
    ./imagesys.cpp
    ./metasys.cpp
    ./orgsys.cpp
    ./wksp.cpp
    ./wksp/workspace.cpp
)

add_library(yqdb SHARED ${db_sources})
target_link_libraries(yqdb PUBLIC yq)

#   override for temp
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/garbage)
add_executable( link_test_yqdb linktest.cpp )
target_link_libraries( link_test_yqdb yqdb )


