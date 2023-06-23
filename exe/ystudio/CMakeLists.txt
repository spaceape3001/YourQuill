################################################################################
##
##  YOUR QUILL
##
################################################################################

project(ystudio)

find_package(Qt6 REQUIRED Core Gui Widgets Network WebEngineCore WebEngineWidgets)

set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} 
    #Browser.hpp
    #Browser.cpp
    DreamApp.hpp 
    DreamApp.cpp 
    DreamMW.hpp 
    DreamMW.cpp
    #FileTree.hpp
    #FileTree.cpp
    #FolderViewIcons.hpp
    #FolderViewIcons.cpp
    main.cpp 
)

target_link_libraries(${PROJECT_NAME} 
    gluon 
    mithril 
    mithrilApp
    mithrilQt
    YQResources 
    Qt6::Network
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
)


