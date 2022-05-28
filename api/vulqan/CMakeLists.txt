add_library(yqvulqan SHARED 
    ./VqCore.cpp
    ./VqCore.hpp
    ./VqInstance.hpp
    ./VqInstance.cpp
)

target_link_libraries(yqvulqan PUBLIC ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} yqbasic )

LinkTest(yqvulqan)

