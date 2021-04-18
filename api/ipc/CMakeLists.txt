SET(ipc_sources
    ./ipcPackets.hpp
    ./ipcSocket.cpp
    ./ipcSocket.hpp
)
add_library(yqipc SHARED ${ipc_sources})

