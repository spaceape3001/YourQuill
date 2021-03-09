set(dbgui_sources
    ./Providers.cpp
    
    ./QuillApp.cpp
    ./QuillApp.hpp
    
    ./atomsys/AtomProvider.hpp
    ./atomsys/AtomTableBase.cpp
    ./atomsys/AtomTableModel.hpp
    ./atomsys/AtomTableView.hpp
    ./atomsys/ClassProvider.hpp
    ./atomsys/ClassTableBase.cpp
    ./atomsys/ClassTableModel.hpp
    ./atomsys/ClassTableView.hpp
    ./atomsys/FieldProvider.hpp
    ./atomsys/FieldTableBase.cpp
    ./atomsys/FieldTableModel.hpp
    ./atomsys/FieldTableView.hpp
    
    ./edit/CopyrightEdit.cpp
    ./edit/CopyrightEdit.hpp
    
    ./filesys/DirectoryProvider.hpp
    ./filesys/DirectoryTableBase.cpp
    ./filesys/DirectoryTableModel.hpp
    ./filesys/DirectoryTableView.hpp
    ./filesys/DocumentProvider.hpp
    ./filesys/DocumentTableBase.cpp
    ./filesys/DocumentTableModel.hpp
    ./filesys/DocumentTableView.hpp
    ./filesys/FolderProvider.hpp
    ./filesys/FolderTableBase.cpp
    ./filesys/FolderTableModel.hpp
    ./filesys/FolderTableView.hpp
    ./filesys/FragmentProvider.hpp
    ./filesys/FragmentTableBase.cpp
    ./filesys/FragmentTableModel.hpp
    ./filesys/FragmentTableView.hpp
    
    ./leaf/LeafProvider.hpp
    ./leaf/LeafTableBase.cpp
    ./leaf/LeafTableModel.hpp
    ./leaf/LeafTableView.hpp
    
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
    ./wksp/RootProvider.hpp
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

