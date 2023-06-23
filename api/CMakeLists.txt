################################################################################
##
##  YOUR QUILL
##
################################################################################

add_subdirectory(mithril)
if(Feature_Qt6)
    add_subdirectory(mithrilQt)
    add_subdirectory(mithrilApp)
endif()

