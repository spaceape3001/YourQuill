################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(basic_test_source ./test.cpp)
endif()

set(basic_sources
    ${basic_test_source}
    ./basic.cpp
)

add_library(yqbasic SHARED ${basic_sources})

target_compile_definitions(yqbasic PRIVATE 
    YQ__CORE__OVERRIDE 
)

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqbasic 
    PUBLIC
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        ${GCC_FSLIB}
        ${SQLite3_LIBRARY}
)

LinkTest(yqbasic)

