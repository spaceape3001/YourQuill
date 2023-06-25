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
    Dock.hpp

    DreamApp.hpp 
    DreamApp.cpp 
    DreamMW.hpp 
    DreamMW.cpp

    #Explorer.cpp
    #Explorer.hpp

    WebBrowser.hpp
    WebBrowser.cpp
    Window.hpp

    atom/atom.cpp

    dock/XFiles.hpp
    dock/XFiles.cpp
    dock/XIdBoard.hpp
    dock/XIdBoard.cpp

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

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)


