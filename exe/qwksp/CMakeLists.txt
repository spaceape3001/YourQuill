set(qwksp_sources 
    ./QWksp.cpp
    ./QWksp.hpp
)
add_executable(qwksp ${qwksp_sources})
target_link_libraries(qwksp PUBLIC yqdb yqdbgui yqgui yqres)

