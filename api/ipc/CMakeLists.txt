SET(ipc_sources
    ./DirWatcher.hpp
    ./DirWatcher.inl
    ./ipcPackets.hpp
    ./ipcSocket.hpp
    ./ipcSocket.inl
    ./PidFile.hpp
    ./PidFile.inl
    ./TmpDirs.hpp
    ./TmpDirs.inl
    ./ipcUnity.cpp
)
add_library(yqipc SHARED ${ipc_sources})
target_link_libraries(yqipc PUBLIC yqutil)

