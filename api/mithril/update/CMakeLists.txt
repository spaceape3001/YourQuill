################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This library should *ONLY* be linked by plugins doing updates, nobody else.

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(update_test_source ./test.cpp)
endif()

add_library(yqupdate SHARED
    ${update_test_source}
    ./update.cpp
)

target_include_directories( yqupdate 
    PRIVATE 
        ${ImageMagick_INCLUDE_DIRS}
) 

target_link_libraries(yqupdate
    PUBLIC 
        yqtoolbox
        yqkernel
    PRIVATE
        ${ImageMagick_LIBRARIES}
)

LinkTest(yqupdate)

