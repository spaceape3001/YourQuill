################################################################################
##
##  YOUR QUILL
##
################################################################################

project(photon)

add_library(${PROJECT_NAME} SHARED
    photon.cpp
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

