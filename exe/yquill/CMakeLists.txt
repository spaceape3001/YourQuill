add_executable(yquill
    #./Explorer.cpp
    #./Explorer.hpp
    ./YWeb.cpp
    ./YWeb.hpp

    ./atom/YAtomTable.cpp
    ./atom/YAtomTable.hpp
    #./class/YClassEditor.cpp
    #./class/YClassEditor.hpp
    ./class/YClassTable.cpp
    ./class/YClassTable.hpp
    ./directory/YDirectoryTable.cpp
    ./directory/YDirectoryTable.hpp
    ./document/YDocumentTable.cpp
    ./document/YDocumentTable.hpp
    ./field/YFieldTable.cpp
    ./field/YFieldTable.hpp
    ./folder/YFolderTable.cpp
    ./folder/YFolderTable.hpp
    ./fragment/YFragmentTable.cpp
    ./fragment/YFragmentTable.hpp
    #./leaf/YLeafEditor.cpp
    #./leaf/YLeafEditor.hpp
    ./leaf/YLeafTable.cpp
    ./leaf/YLeafTable.hpp
    #./tag/YTagEditor.cpp
    #./tag/YTagEditor.hpp
    ./tag/YTagTable.cpp
    ./tag/YTagTable.hpp

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


