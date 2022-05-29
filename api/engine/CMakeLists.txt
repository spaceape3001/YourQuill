if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(engine_test_source ./test.cpp)
endif()

set(yq_engine_sources
    ${engine_test_source}
    
    ./VqCore.cpp
    ./VqCore.hpp
    ./VqEnums.cpp
    ./VqGLFW.cpp
    ./VqGLFW.hpp
    ./VqInstance.cpp
    ./VqInstance.hpp
    ./VqMonitor.cpp
    ./VqMonitor.hpp
    ./VqWindow.cpp
    ./VqWindow.hpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_link_libraries(yqengine PUBLIC yqbasic yqconfig ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY})
LinkTest(yqengine)


