add_executable(yquill
    #./Explorer.cpp
    #./Explorer.hpp
    ./YWeb.cpp
    ./YWeb.hpp

    ./atomsys/YAtomTable.cpp
    ./atomsys/YAtomTable.hpp
    #./atomsys/YClassEditor.cpp
    #./atomsys/YClassEditor.hpp
    ./atomsys/YClassTable.cpp
    ./atomsys/YClassTable.hpp
    ./atomsys/YFieldTable.cpp
    ./atomsys/YFieldTable.hpp
    ./filesys/YDirectoryTable.cpp
    ./filesys/YDirectoryTable.hpp
    ./filesys/YDocumentTable.cpp
    ./filesys/YDocumentTable.hpp
    ./filesys/YFolderTable.cpp
    ./filesys/YFolderTable.hpp
    ./filesys/YFragmentTable.cpp
    ./filesys/YFragmentTable.hpp
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


