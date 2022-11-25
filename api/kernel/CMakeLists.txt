################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(kernel_test_source ./test.cpp)
endif()

add_library(yqkernel SHARED
    ${kernel_test_source}
    ./kernel.cpp
    ./wksp/Workspace.cpp
)

target_link_libraries(yqkernel
    PUBLIC 
        yqtoolbox

    PRIVATE
        RapidXml
)

target_compile_definitions(yqkernel 
    PRIVATE
        -DDEV_BUILD=1
        "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
        "-DYQ_SHARE_DIRS=\"${YOUR_QUILL_KERNEL}/share\;${CMAKE_SOURCE_DIR}/share\"" 
)

LinkTest(yqkernel)

