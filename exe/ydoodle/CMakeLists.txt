################################################################################
##
##  YOUR QUILL
##
################################################################################

project(ydoodle)

find_package(Qt6 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}
    main.cpp
    DoodleMW.hpp
    DoodleMW.cpp
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        yqtoolbox
        gluon
        doodler
        doodlerQt
        yqresources
)


    
