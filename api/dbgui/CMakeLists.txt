set(dbgui_sources
    ./QuillApp.cpp
    ./QuillApp.hpp
    
    ./wksp/RootComboBox.cpp
    ./wksp/RootComboBox.hpp
    ./wksp/WkspSelectorDlg.cpp
    ./wksp/WkspSelectorDlg.hpp
)

add_library(yqdbgui SHARED ${dbgui_sources})
target_link_libraries(yqdbgui PUBLIC yqdb yqmeta yqutil Qt5::Widgets)
