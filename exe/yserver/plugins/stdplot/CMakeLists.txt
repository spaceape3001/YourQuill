################################################################################
##
##  YOUR QUILL
##
################################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PLUGIN_DIR}/plot)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_DIR}/plot)

add_library( stdplot SHARED
    ./page.cpp
    ./update.cpp
)
target_include_directories( stdplot PRIVATE . ) 
target_link_libraries( stdplot 
    PUBLIC 
        yq_mithril
        yq_asio
)

LinkTest(stdplot)

