add_executable(ipc_hello ipc_hello.cpp)
target_link_libraries(ipc_hello PUBLIC yqdb yqipc)

add_executable(ipc_restart ipc_restart.cpp)
target_link_libraries(ipc_restart PUBLIC yqdb yqipc)

add_executable(ipc_shutdown ipc_shutdown.cpp)
target_link_libraries(ipc_shutdown PUBLIC yqdb yqipc)

