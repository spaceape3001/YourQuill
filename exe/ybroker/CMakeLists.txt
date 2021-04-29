add_executable(ybroker ./yBroker.cpp)
target_link_libraries(ybroker PUBLIC yqdb yqipc yqutil)

