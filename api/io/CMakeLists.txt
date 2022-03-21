#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(io_test_source ./test.cpp)
endif()

set(io_sources 
    ${io_test_source}

    ./http/HttpData.hpp
    ./http/HttpParser.hpp
    ./http/HttpRequest.hpp
    ./http/HttpUnity.cpp
    
    ./web/WebPage.hpp
    ./web/WebUnity.cpp
)


add_library( yqio SHARED ${io_sources})
target_link_libraries(yqio PUBLIC yqutil)

