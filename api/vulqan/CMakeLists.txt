add_library(yqvulqan SHARED 
    ./VqCore.cpp
    ./VqCore.hpp
    ./VqInstance.cpp
    ./VqInstance.hpp
    ./VqWindow.cpp
    ./VqWindow.hpp
)

target_link_libraries(yqvulqan PUBLIC ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} yqbasic )

LinkTest(yqvulqan)

