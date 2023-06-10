project(ystudio)

find_package(Qt5 REQUIRED Core Gui Widgets Network WebEngineWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} 
    Browser.hpp
    Browser.cpp
    DreamApp.hpp 
    DreamApp.cpp 
    DreamMW.hpp 
    DreamMW.cpp
    FileTree.hpp
    FileTree.cpp
    FolderViewIcons.hpp
    FolderViewIcons.cpp
    IdEncoding.hpp 
    IdEncoding.cpp 
    main.cpp 
)

target_link_libraries(${PROJECT_NAME} gluon mithril YQResources Qt5::Network Qt5::WebEngineWidgets)


