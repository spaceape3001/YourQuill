add_executable(yquill
    #./ClassEditor.cpp
    #./ClassEditor.hpp
    #./Explorer.cpp
    #./Explorer.hpp
    #./LeafEditor.cpp
    #./LeafEditor.hpp
    #./LeafTable.cpp
    #./LeafTable.hpp
    #./YTagEditor.cpp
    #./YTagEditor.hpp
    ./YClassTable.cpp
    ./YClassTable.hpp
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


