add_executable(yquill
    #./ClassEditor.cpp
    #./ClassEditor.hpp
    #./ClassTable.cpp
    #./ClassTable.hpp
    #./Explorer.cpp
    #./Explorer.hpp
    #./LeafEditor.cpp
    #./LeafEditor.hpp
    #./LeafTable.cpp
    #./LeafTable.hpp
    #./TagEditor.cpp
    #./TagEditor.hpp
    #./TagTable.cpp
    #./TagTable.hpp

    ./YWeb.cpp
    ./YWeb.hpp

    ./YourQuill.cpp
    ./YourQuill.hpp
)

target_link_libraries(yquill PUBLIC
    yqdb
    yqdbgui 
    yqgui
    yqmeta
    yqutil
)


