if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(engine_test_source ./test.cpp)
endif()

set(yq_engine_sources
    ${engine_test_source}
    
    ./engine.cpp
    ./preamble.hpp
    ./Visualizer.cpp
    ./Visualizer.hpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_link_libraries(yqengine PUBLIC yqbasic)
LinkTest(yqengine)


