################################################################################
##
##  YOUR QUILL
##
################################################################################

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

target_link_libraries(yqupdate
    PUBLIC 
        yqtoolbox
        yqkernel
)

target_include_directories(yqupdate 
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/..
)

LinkTest(yqupdate)

