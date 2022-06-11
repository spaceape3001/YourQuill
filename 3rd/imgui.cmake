add_library(imgui SHARED 
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_vulkan.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/misc/cpp/imgui_stdlib.cpp
    #imgui/misc/freetype/imgui_freetype.cpp
)

target_include_directories(imgui PUBLIC ./imgui)

target_link_libraries(imgui PUBLIC 
    ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY}
)
    
LinkTest(imgui)


