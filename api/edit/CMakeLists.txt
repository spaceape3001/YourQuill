SET(edit_sources 
    ./Editor.cpp
    ./Editor.hpp
    ./EditorInfoWriter.hpp
)

add_library(yqedit SHARED ${edit_sources})
target_link_libraries(yqedit PUBLIC yq yqengine)

