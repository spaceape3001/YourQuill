################################################################################
##
##  YOUR QUILL
##
################################################################################

project(mithrilQt)


find_package(Qt6 COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)
#find_package(Qt5 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)

add_library(${PROJECT_NAME} SHARED
    id/IdColumn.hpp
    id/IdColumn.ipp
    id/IdListModel.hpp
    id/IdListModel.ipp
    id/IdModel.hpp
    id/IdModel.ipp
    id/IdTableModel.hpp
    id/IdTableModel.ipp
    id/IdItemModel.hpp
    id/IdItemModel.ipp
    id/id.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
        mithril
        gluon
        
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

