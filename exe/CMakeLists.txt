################################################################################
##
##  YOUR QUILL
##
################################################################################

# add_subdirectory(3x)
add_subdirectory(yserver)

if(Build_Qt6)
    add_subdirectory(yeditor)
    #add_subdirectory(ydoodle)
endif()

