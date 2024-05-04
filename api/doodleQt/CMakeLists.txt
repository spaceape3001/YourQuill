################################################################################
##
##  YOUR QUILL
##
################################################################################

project(doodleQt)

find_package(Qt6 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME} SHARED
    doodleQt.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
        doodle
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
)

LinkTest(${PROJECT_NAME})

