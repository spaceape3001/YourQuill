SET(ipc_sources
    ./DirWatcher.cpp
    ./DirWatcher.hpp
    ./ipcPackets.hpp
    ./ipcSocket.cpp
    ./ipcSocket.hpp
)
add_library(yqipc SHARED ${ipc_sources})
target_link_libraries(yqipc PUBLIC yqutil)

