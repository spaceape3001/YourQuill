set(gadget_sources
    ./DateGen.cpp
    ./DateGen.hpp
    ./MonthView.cpp
    ./MonthView.hpp
    ./SqlTool.cpp
    ./SqlTool.hpp
    ./TimeGen.cpp
    ./TimeGen.hpp
)

add_library(yqgadget SHARED ${gadget_sources})
target_link_libraries(yqgadget PUBLIC yqgui yqutil)


