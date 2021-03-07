set(ywksp_sources 
    ./YWksp.cpp
    ./YWksp.hpp
)
add_executable(ywksp ${ywksp_sources})
target_link_libraries(ywksp PUBLIC yqdb yqdbgui yqgui yqres)

