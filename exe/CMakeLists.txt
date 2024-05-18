################################################################################
##
##  YOUR QUILL
##
################################################################################

add_subdirectory(3x)
add_subdirectory(yserver)

if(Feature_Vulkan)
endif()

if(Feature_Qt6)
    add_subdirectory(yeditor)
    add_subdirectory(ydoodle)
endif()

