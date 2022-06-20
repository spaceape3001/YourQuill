################################################################################
##
##  YOUR QUILL
##
################################################################################

#   Library for any/all IMGUI widgets

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(imgui_test_source ./test.cpp)
endif()

set(imgui_sources
    ${imgui_test_source}
    ./imgui.cpp
)

add_library(yqimgui SHARED ${imgui_sources})
target_link_libraries(yqimgui PUBLIC imgui)


