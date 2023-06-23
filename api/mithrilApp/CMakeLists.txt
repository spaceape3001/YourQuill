################################################################################
##
##  YOUR QUILL
##
################################################################################

project(mithrilApp)

find_package(Qt6 COMPONENTS Core Gui Widgets Network WebEngineCore WebEngineWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(mithrilApp SHARED
    web/WebBrowser.hpp
    web/WebBrowser.ipp
    web/web.cpp
    mithrilApp.qrc
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    gluon 
    mithril 
    mithrilQt
    yqtoolbox
    Qt6::Network
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
)

