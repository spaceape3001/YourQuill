add_executable(yquill
    #./ClassEditor.cpp
    #./ClassEditor.hpp
    #./Explorer.cpp
    #./Explorer.hpp
    #./LeafEditor.cpp
    #./LeafEditor.hpp
    #./YTagEditor.cpp
    #./YTagEditor.hpp
    ./YAtomTable.cpp
    ./YAtomTable.hpp
    ./YClassTable.cpp
    ./YClassTable.hpp
    ./YDirectoryTable.cpp
    ./YDirectoryTable.hpp
    ./YDocumentTable.cpp
    ./YDocumentTable.hpp
    ./YFieldTable.cpp
    ./YFieldTable.hpp
    ./YFolderTable.cpp
    ./YFolderTable.hpp
    ./YFragmentTable.cpp
    ./YFragmentTable.hpp
    ./YLeafTable.cpp
    ./YLeafTable.hpp
    ./YTagTable.cpp
    ./YTagTable.hpp
    ./YWeb.cpp
    ./YWeb.hpp
    ./YourQuill.cpp
    ./YourQuill.hpp
)

target_link_libraries(yquill PUBLIC
    yqdb
    yqdbgui 
    yqgadget
    yqgui
    yqmeta
    yqutil
)


