################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(earth_test_source ./test.cpp)
endif()

set(earth_sources
    ${earth_test_source}
    ./earth.cpp
)

add_library(yqearth SHARED ${earth_sources})

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqearth 
    PUBLIC
        yqbasic
        yqmath
)

LinkTest(yqearth)

