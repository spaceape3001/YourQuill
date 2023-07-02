################################################################################
##
##  YOUR QUILL
##
################################################################################

add_subdirectory(photon)
add_subdirectory(mithril)
if(Feature_Qt6)
    add_subdirectory(photonQt)
    add_subdirectory(mithrilQt)
endif()

