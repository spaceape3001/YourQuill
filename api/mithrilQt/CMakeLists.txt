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

    mithrilQt.cpp

    id/IdListView.hpp
    id/IdMimeList.hpp
    id/IdModel.hpp
    id/IdSignalMapper.hpp
    id/IdTableView.hpp
    id/IdTreeView.hpp
    
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
        ${CMAKE_CURRENT_LIST_DIR}/..
    PRIVATE
        ${TBB_INCLUDE_DIR}
        ${LOG4CPP_INCLUDE_DIR}
)

LinkTest(${PROJECT_NAME})

