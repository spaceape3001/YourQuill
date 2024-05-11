################################################################################
##
##  YOUR QUILL
##
################################################################################

project(mithril)

add_library(${PROJECT_NAME} SHARED
    all_arg.cpp
    all_cdb.cpp
    all_html.cpp
    all_ipc.cpp
    all_meta.cpp
    all_provider.cpp
    bit.cpp
    directories.cpp
    doodle.cpp
    mithril.cpp
    notify.cpp
    html/XEntry.cpp
    web/web_unity.cpp
    wksp/Workspace.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
    PRIVATE
        asio
        #${ImageMagick_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
    #PRIVATE
        #${ImageMagick_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -DDEV_BUILD=1
        "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
        "-DYQ_SHARE_DIRS=\"${CMAKE_SOURCE_DIR}/share\"" 
)

LinkTest(${PROJECT_NAME})

