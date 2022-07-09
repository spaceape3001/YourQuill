################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(physics_test_source ./test.cpp)
endif()

set(physics_sources
    ${physics_test_source}
    ./physics.cpp
)

add_library(yqphysics SHARED ${physics_sources})

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqphysics 
    PUBLIC
        yqbasic
        yqmath
)

LinkTest(yqphysics)

