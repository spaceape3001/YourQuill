################################################################################
##
##  YOUR QUILL
##
################################################################################


add_subdirectory(yserver)

if(Feature_Qt6)
    add_subdirectory(yeditor)
    add_subdirectory(ydoodle)
endif()

