add_executable(ywksp 
    ./YWksp.cpp
    ./YWksp.hpp
)
target_link_libraries(ywksp PUBLIC yqdb yqdbgui yqgui yqres)

