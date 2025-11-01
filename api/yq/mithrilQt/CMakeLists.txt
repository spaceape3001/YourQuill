################################################################################
##
##  YOUR QUILL
##
################################################################################

project(yq_mithrilQt)


find_package(Qt6 COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)
#find_package(Qt5 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME} SHARED

    atom.cpp
    attribute.cpp
    book.cpp
    category.cpp
    character.cpp
    class.cpp
    directory.cpp
    document.cpp
    doodle.cpp
    entity.cpp
    event.cpp
    field.cpp
    folder.cpp
    fragment.cpp
    game.cpp
    graphviz.cpp
    game.cpp
    group.cpp
    id.cpp
    id_auto.cpp
    image.cpp
    leaf.cpp
    meta.cpp
    novel.cpp
    organization.cpp
    place.cpp
    root.cpp
    style.cpp
    tag.cpp
    thing.cpp
    user.cpp
    value.cpp
    
    id/IdColumn.cpp
    id/IdListView.hpp
    id/IdListView.cpp
    id/IdMimeList.hpp
    id/IdMimeList.cpp
    id/IdModel.hpp
    id/IdModel.cpp
    id/IdSignalMapper.hpp
    id/IdSignalMapper.cpp
    id/IdTableView.hpp
    id/IdTableView.cpp
    id/IdTreeView.hpp
    id/IdTreeView.cpp
    
    mithrilQt.qrc
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yq_gluon
        yq_mithril
        
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets

    PRIVATE
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${YQUILL_API_DIR}
    PRIVATE
        ${TBB_INCLUDE_DIR}
        ${LOG4CPP_INCLUDE_DIR}
)

LinkTest(${PROJECT_NAME})

