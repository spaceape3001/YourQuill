################################################################################
##
##  YOUR QUILL
##
################################################################################

add_subdirectory(b3)
add_subdirectory(doodler)
add_subdirectory(noveler)
add_subdirectory(mithril)

if(Feature_Vulkan)
    #   Add Vulkan things
endif()

if(Feature_Qt6)
    add_subdirectory(doodlerQt)
    add_subdirectory(novelerQt)
    add_subdirectory(mithrilQt)
endif()

