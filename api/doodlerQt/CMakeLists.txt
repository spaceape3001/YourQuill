################################################################################
##
##  YOUR QUILL
##
################################################################################

project(doodlerQt)

find_package(Qt6 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME} SHARED
    doodleQt.cpp
    QProject.cpp
    QProject.hpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        doodler
        gluon
        
        Qt6::Core 
        Qt6::Gui 
        Qt6::Widgets
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
)

LinkTest(${PROJECT_NAME})

