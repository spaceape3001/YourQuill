################################################################################
##
##  YOUR QUILL
##
################################################################################

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(math_test_source ./test.cpp)
endif()

set(yq_math_sources
    ${math_test_source}
    
    ./math.cpp
    ./preamble.hpp
)

add_library(yqmath SHARED ${yq_math_sources})
target_link_libraries(yqmath PUBLIC yqbasic)
LinkTest(yqmath)


