set(dbgui_sources
    ./QuillApp.cpp
    ./QuillApp.hpp
)

add_library(yqdbgui SHARED ${dbgui_sources})
target_link_libraries(yqdbgui PUBLIC yqdb yqmeta yqutil Qt5::Widgets)

