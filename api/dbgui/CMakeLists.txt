set(dbgui_sources
    ./Provider.hpp
    ./Providers.cpp
    
    ./QuillApp.cpp
    ./QuillApp.hpp
    
    ./edit/CopyrightEdit.cpp
    ./edit/CopyrightEdit.hpp
    
    ./quill/QuillDataEdit.cpp
    ./quill/QuillDataEdit.hpp
    ./quill/QuillDirTable.cpp
    ./quill/QuillDirTable.hpp
    ./quill/QuillEdit.cpp
    ./quill/QuillEdit.hpp
    
    ./tag/TagEdit.cpp
    ./tag/TagEdit.hpp
    ./tag/TagProvider.hpp
    ./tag/TagTableBase.cpp
    ./tag/TagTableModel.hpp
    ./tag/TagTableView.hpp
    
    ./web/WebBase.cpp
    ./web/WebBase.cpp
    
    ./wksp/RootComboBox.cpp
    ./wksp/RootComboBox.hpp
    ./wksp/WkspSelectorDlg.cpp
    ./wksp/WkspSelectorDlg.hpp
)

add_library(yqdbgui SHARED ${dbgui_sources})
target_link_libraries(yqdbgui 
    PUBLIC 
        yqdb 
        yqgui
        yqmeta 
        yqres
        yqutil 
        Qt5::Widgets 
        Qt5::WebEngine 
        Qt5::WebEngineWidgets
)

