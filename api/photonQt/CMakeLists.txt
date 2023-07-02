################################################################################
##
##  YOUR QUILL
##
################################################################################

project(photonQt)


find_package(Qt6 COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME} SHARED

    photonQt.cpp
    #photonQt.qrc
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
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

