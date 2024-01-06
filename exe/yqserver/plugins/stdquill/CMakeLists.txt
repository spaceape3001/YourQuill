################################################################################
##
##  YOUR QUILL
##
################################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PLUGIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_DIR})

set(stdquill_SOURCES 
    #./imaging.cpp
    #common.cpp
    ./variables.cpp
    ./page.cpp
    ./update.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
target_include_directories( stdquill 
    PRIVATE 
        . 
        #${ImageMagick_INCLUDE_DIRS}
) 
target_link_libraries( stdquill 
    PUBLIC 
        yqtoolbox 
        # yqupdate
        mithril
        asio
        #${ImageMagick_LIBRARIES}
)

LinkTest(stdquill)

