################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(http_test_source ./test.cpp)
endif()

add_library(yqhttp SHARED
    ${http_test_source}
    ./http.cpp
    ./XEntry.cpp
    ./web/web_unity.cpp
)

target_link_libraries(yqhttp
    PUBLIC 
        yqtoolbox
        yqkernel
    PRIVATE
        asio
)

target_include_directories(yqhttp 
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/..
)

LinkTest(yqhttp)

