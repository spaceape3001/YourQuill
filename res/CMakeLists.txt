project(YQResources)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core)
add_library(
    ${PROJECT_NAME} SHARED 
        YQResources.cpp 
        yquill.qrc
)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)

