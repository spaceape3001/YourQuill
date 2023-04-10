################################################################################
##
##  YOUR QUILL
##
################################################################################

project(mithril)

add_library(${PROJECT_NAME} SHARED
    mithril/mithril.cpp
    mithril/http/XEntry.cpp
    mithril/web/web_unity.cpp
    mithril/wksp/Workspace.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
    PRIVATE
        asio
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -DDEV_BUILD=1
        "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
        "-DYQ_SHARE_DIRS=\"${CMAKE_SOURCE_DIR}/share\"" 
)

LinkTest(${PROJECT_NAME})

