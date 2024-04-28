################################################################################
##
##  YOUR QUILL
##
################################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PLUGIN_DIR}/plot)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_DIR}/plot)

set(stdplot_SOURCES 
    ./page.cpp
    ./update.cpp
)

add_library( stdplot SHARED ${stdplot_SOURCES} )
target_include_directories( stdplot PRIVATE . ) 
target_link_libraries( stdplot 
    PUBLIC 
        yqtoolbox 
        mithril
        asio
)

LinkTest(stdplot)

