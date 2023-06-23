################################################################################
##
##  YOUR QUILL
##
################################################################################

project(ystudio)

find_package(Qt6 COMPONENTS Core Gui Widgets Network WebEngineCore WebEngineWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} 
    DreamApp.hpp 
    DreamApp.cpp 
    DreamMW.hpp 
    DreamMW.cpp

    #Explorer.cpp
    #Explorer.hpp

    WebBrowser.hpp
    WebBrowser.cpp

    XFiles.hpp
    XFilesImpl.hpp
    XFiles.cpp

    main.cpp 
    ystudio.qrc
)

target_link_libraries(${PROJECT_NAME} 
    gluon 
    mithril 
    mithrilQt
    yqtoolbox
    Qt6::Network
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
)


