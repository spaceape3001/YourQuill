################################################################################
##
##  YOUR QUILL
##
################################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PLUGIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_DIR})

add_library( b3web SHARED
    pages.cpp
)

target_include_directories( b3web PRIVATE . ) 
target_link_libraries( b3web 
    PUBLIC 
        yq_b3
        yq_mithril
)

LinkTest(b3web)
