add_executable(yquill
    ./YExplorer.cpp
    ./YExplorer.hpp
    ./YTableUnity.cpp
    ./YWeb.cpp
    ./YWeb.hpp

    ./atom/YAtomTable.hpp
    ./atom/YAtomTable.ipp
    ./attribute/YAttributeTable.hpp
    ./attribute/YAttributeTable.ipp
    #./class/YClassEditor.cpp
    #./class/YClassEditor.hpp
    ./class/YClassTable.hpp
    ./class/YClassTable.ipp
    ./directory/YDirectoryTable.hpp
    ./directory/YDirectoryTable.ipp
    ./document/YDocumentTable.hpp
    ./document/YDocumentTable.ipp
    ./field/YFieldTable.hpp
    ./field/YFieldTable.ipp
    ./folder/YFolderTable.hpp
    ./folder/YFolderTable.ipp
    ./fragment/YFragmentTable.hpp
    ./fragment/YFragmentTable.ipp
    #./leaf/YLeafEditor.cpp
    #./leaf/YLeafEditor.hpp
    ./leaf/YLeafTable.hpp
    ./leaf/YLeafTable.ipp
    ./property/YPropertyTable.hpp
    ./property/YPropertyTable.ipp
    #./tag/YTagEditor.cpp
    #./tag/YTagEditor.hpp
    ./tag/YTagTable.hpp
    ./tag/YTagTable.ipp

    ./YourQuill.cpp
    ./YourQuill.hpp
)

target_link_libraries(yquill PRIVATE
    yqdb
    yqdbgui 
    yqgadget
    yqgui
    yqmeta
    yqutil
)

target_include_directories(yquill PRIVATE .)

