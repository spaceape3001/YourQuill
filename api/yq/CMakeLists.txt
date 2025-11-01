################################################################################
##
##  YOUR QUILL
##
################################################################################

#add_subdirectory(noveler)
add_subdirectory(mithril)

if(Build_Vulkan)
    #   Add Vulkan things
    add_subdirectory(mithrilVk)
endif()

if(Build_Qt6)
    #add_subdirectory(novelerQt)
    add_subdirectory(mithrilQt)
endif()

