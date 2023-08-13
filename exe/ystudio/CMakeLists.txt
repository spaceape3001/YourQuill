################################################################################
##
##  YOUR QUILL
##
################################################################################

project(ystudio)

find_package(Qt6 COMPONENTS Core Gui Widgets Network Svg SvgWidgets WebEngineCore WebEngineWidgets)

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

    IdList.hpp
    IdList.cpp
    IdTable.hpp
    IdTable.cpp

    WebBrowser.hpp
    WebBrowser.cpp
    Window.hpp

    dock/DateGen.hpp
    dock/DateGen.cpp

    dock/XDocuments.hpp
    dock/XDocuments.cpp
    dock/XFragments.hpp
    dock/XFragments.cpp
    dock/XIdBoard.hpp
    dock/XIdBoard.cpp
    
    viewer/ImageViewer.hpp
    viewer/ImageViewer.cpp
    viewer/SvgViewer.hpp
    viewer/SvgViewer.cpp

    wksp/atom.cpp
    wksp/attribute.cpp
    wksp/book.cpp
    wksp/category.cpp
    wksp/character.cpp
    wksp/class.cpp
    wksp/directory.cpp
    wksp/document.cpp
    wksp/entity.cpp
    wksp/event.cpp
    wksp/field.cpp
    wksp/folder.cpp
    wksp/fragment.cpp
    wksp/game.cpp
    wksp/group.cpp
    wksp/leaf.cpp
    wksp/organization.cpp
    wksp/place.cpp
    wksp/root.cpp
    wksp/tag.cpp
    wksp/user.cpp

    main.cpp 
    ystudio.qrc

)

target_link_libraries(${PROJECT_NAME} 
    gluon 
    mithril 
    mithrilQt
    yqtoolbox
    Qt6::Network
    Qt6::Svg
    Qt6::SvgWidgets
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)


