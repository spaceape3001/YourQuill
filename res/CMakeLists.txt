################################################################################
##
##  YOUR QUILL
##
################################################################################

project(yq_resources)

find_package(Qt6 COMPONENTS Core Gui Widgets)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME} SHARED yqResources.cpp yqResources.qrc)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
target_include_directories(${PROJECT_NAME} PUBLIC . )

