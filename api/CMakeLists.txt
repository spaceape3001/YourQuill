################################################################################
##
##  YOUR QUILL
##
################################################################################

add_subdirectory(doodler)
add_subdirectory(noveler)
add_subdirectory(mithril)

if(Build_Vulkan)
    #   Add Vulkan things
endif()

if(Build_Qt6)
    add_subdirectory(doodlerQt)
    add_subdirectory(novelerQt)
    add_subdirectory(mithrilQt)
endif()

