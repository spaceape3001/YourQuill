set(gadget_sources
    ./DateGen.hpp
    ./DateGen.ipp
    ./GadgetUnity.cpp
    ./MonthView.hpp
    ./MonthView.ipp
    ./SqlTool.hpp
    ./SqlTool.ipp
    ./TimeGen.hpp
    ./TimeGen.ipp
)

add_library(yqgadget SHARED ${gadget_sources})
target_link_libraries(yqgadget 
    PUBLIC 
        yqgui 
        yqutil
        Qt5::Core
        Qt5::Widgets
)


