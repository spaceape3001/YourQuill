################################################################################
##
##  YOUR QUILL
##
################################################################################

set(stdquill_SOURCES 
    ./imaging.cpp
    #common.cpp
    ./variables.cpp
    ./page/page.cpp
    ./update/update.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
target_include_directories( stdquill PRIVATE . ${ImageMagick_INCLUDE_DIRS}) 
target_link_libraries( stdquill PUBLIC yqtoolbox yqkernel yqhttp asio ${ImageMagick_LIBRARIES})

LinkTest(stdquill)
