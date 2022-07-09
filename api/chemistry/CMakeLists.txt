################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(chemistry_test_source ./test.cpp)
endif()

set(chemistry_sources
    ${chemistry_test_source}
    ./chemistry.cpp
)

add_library(yqchemistry SHARED ${chemistry_sources})

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqchemistry 
    PUBLIC
        yqbasic
        yqmath
)

LinkTest(yqchemistry)

