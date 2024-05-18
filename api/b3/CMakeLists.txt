################################################################################
##
##  YOUR QUILL
##
################################################################################

#   B3 is the library for the command line map/blueprint processor 3x

project(b3)

add_library(${PROJECT_NAME} SHARED
    b3.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
)

LinkTest(${PROJECT_NAME})

