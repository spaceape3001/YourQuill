################################################################################
##
##  YOUR QUILL
##
################################################################################

add_subdirectory(doodle)
add_subdirectory(noveler)
add_subdirectory(mithril)
if(Feature_Qt6)
    add_subdirectory(doodleQt)
    add_subdirectory(novelerQt)
    add_subdirectory(mithrilQt)
endif()

