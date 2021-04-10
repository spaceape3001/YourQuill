set(dbgui_sources
    ./Providers.cpp
    
    ./QuillApp.cpp
    ./QuillApp.hpp
    
    ./TableUnity.cpp
    
    ./atom/AtomProvider.hpp
    ./atom/AtomQ.hpp
    ./atom/AtomTableBase.ipp
    ./atom/AtomTableModel.hpp
    ./atom/AtomTableView.hpp
    
    ./attribute/AttributeProvider.hpp
    ./attribute/AttributeQ.hpp
    ./attribute/AttributeTableBase.ipp
    ./attribute/AttributeTableModel.hpp
    ./attribute/AttributeTableView.hpp
    
    
    ./class/ClassProvider.hpp
    ./class/ClassQ.hpp
    ./class/ClassTableBase.ipp
    ./class/ClassTableModel.hpp
    ./class/ClassTableView.hpp
    
    ./directory/DirectoryProvider.hpp
    ./directory/DirectoryQ.hpp
    ./directory/DirectoryTableBase.ipp
    ./directory/DirectoryTableModel.hpp
    ./directory/DirectoryTableView.hpp
    
    ./document/DocumentProvider.hpp
    ./document/DocumentQ.hpp
    ./document/DocumentTableBase.ipp
    ./document/DocumentTableModel.hpp
    ./document/DocumentTableView.hpp

    ./edit/CopyrightEdit.cpp
    ./edit/CopyrightEdit.hpp
    
    ./field/FieldProvider.hpp
    ./field/FieldQ.hpp
    ./field/FieldTableBase.ipp
    ./field/FieldTableModel.hpp
    ./field/FieldTableView.hpp

    ./folder/FolderExplorer.cpp
    ./folder/FolderExplorerBase.hpp
    ./folder/FolderExplorerImpl.hpp
    ./folder/FolderProvider.hpp
    ./folder/FolderQ.hpp
    ./folder/FolderTableBase.ipp
    ./folder/FolderTableModel.hpp
    ./folder/FolderTableView.hpp
    
    ./fragment/FragmentProvider.hpp
    ./fragment/FragmentQ.hpp
    ./fragment/FragmentTableBase.ipp
    ./fragment/FragmentTableModel.hpp
    ./fragment/FragmentTableView.hpp
    
    ./graph/GraphQ.hpp
    
    ./image/ImageQ.hpp
    
    ./leaf/LeafProvider.hpp
    ./leaf/LeafQ.hpp
    ./leaf/LeafTableBase.ipp
    ./leaf/LeafTableModel.hpp
    ./leaf/LeafTableView.hpp
    
    ./property/PropertyProvider.hpp
    ./property/PropertyQ.hpp
    ./property/PropertyTableBase.ipp
    ./property/PropertyTableModel.hpp
    ./property/PropertyTableView.hpp
    
    ./quill/QuillDataEdit.ipp
    ./quill/QuillDataEdit.hpp
    ./quill/QuillDirTable.ipp
    ./quill/QuillDirTable.hpp
    ./quill/QuillEdit.cpp
    ./quill/QuillEdit.hpp
    
    ./tag/TagEdit.cpp
    ./tag/TagEdit.hpp
    ./tag/TagProvider.hpp
    ./tag/TagQ.hpp
    ./tag/TagTableBase.ipp
    ./tag/TagTableModel.hpp
    ./tag/TagTableView.hpp
    
    ./web/WebBase.cpp
    ./web/WebBase.cpp
    
    ./wksp/RootComboBox.ipp
    ./wksp/RootComboBox.hpp
    ./wksp/RootProvider.hpp
    ./wksp/WkspSelectorDlg.ipp
    ./wksp/WkspSelectorDlg.hpp
    ./wksp/WkspUnity.cpp
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

