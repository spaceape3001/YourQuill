set(dbgui_sources
    ./Providers.cpp
    
    ./QuillApp.cpp
    ./QuillApp.hpp
    
    ./atom/AtomProvider.hpp
    ./atom/AtomTableBase.cpp
    ./atom/AtomTableModel.hpp
    ./atom/AtomTableView.hpp
    
    ./attribute/AttributeProvider.hpp
    ./attribute/AttributeTableBase.cpp
    ./attribute/AttributeTableModel.hpp
    ./attribute/AttributeTableView.hpp
    
    
    ./class/ClassProvider.hpp
    ./class/ClassTableBase.cpp
    ./class/ClassTableModel.hpp
    ./class/ClassTableView.hpp
    
    ./directory/DirectoryProvider.hpp
    ./directory/DirectoryTableBase.cpp
    ./directory/DirectoryTableModel.hpp
    ./directory/DirectoryTableView.hpp
    ./document/DocumentProvider.hpp
    ./document/DocumentTableBase.cpp
    ./document/DocumentTableModel.hpp
    ./document/DocumentTableView.hpp

    ./edit/CopyrightEdit.cpp
    ./edit/CopyrightEdit.hpp
    
    ./field/FieldProvider.hpp
    ./field/FieldTableBase.cpp
    ./field/FieldTableModel.hpp
    ./field/FieldTableView.hpp

    ./folder/FolderProvider.hpp
    ./folder/FolderTableBase.cpp
    ./folder/FolderTableModel.hpp
    ./folder/FolderTableView.hpp
    ./fragment/FragmentProvider.hpp
    ./fragment/FragmentTableBase.cpp
    ./fragment/FragmentTableModel.hpp
    ./fragment/FragmentTableView.hpp
    
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

