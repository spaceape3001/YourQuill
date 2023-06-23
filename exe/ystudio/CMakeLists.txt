################################################################################
##
##  YOUR QUILL
##
################################################################################

project(ystudio)

find_package(Qt6 REQUIRED Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} 
    DreamApp.hpp 
    DreamApp.cpp 
    DreamMW.hpp 
    DreamMW.cpp
    main.cpp 
    ystudio.qrc
)

target_link_libraries(${PROJECT_NAME} 
    gluon 
    mithril 
    mithrilApp
    mithrilQt
)


