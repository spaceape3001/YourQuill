################################################################################
##
##  YOUR QUILL
##
################################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PLUGIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_DIR})

set(artweb_SOURCES 
    ./pages.cpp
    ./update.cpp
)

add_library( artweb SHARED ${artweb_SOURCES} )
target_link_libraries( artweb
    PUBLIC 
        yq_art
        yq_mithril
        yq_asio
)

LinkTest(artweb)
