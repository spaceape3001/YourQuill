################################################################################
##
##  YOUR QUILL
##
################################################################################


#add_subdirectory(yquill)
add_subdirectory(yeditor)
add_subdirectory(yqserver)

if(Feature_Qt6)
    add_subdirectory(ystudio)
endif()

