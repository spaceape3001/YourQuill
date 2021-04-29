SET(ipc_sources
    ./DirWatcher.hpp
    ./DirWatcher.ipp
    ./ipcPackets.hpp
    ./ipcSocket.hpp
    ./ipcSocket.ipp
    ./PidFile.hpp
    ./PidFile.ipp
    ./ipcUnity.cpp
)
add_library(yqipc SHARED ${ipc_sources})
target_link_libraries(yqipc PUBLIC yqutil)

