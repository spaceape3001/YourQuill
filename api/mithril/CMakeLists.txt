################################################################################
##
##  YOUR QUILL
##
################################################################################

project(mithril)

add_library(${PROJECT_NAME} SHARED
    atom.cpp
    attribute.cpp
    bit.cpp
    book.cpp
    category.cpp
    character.cpp
    class.cpp
    directories.cpp
    directory.cpp
    document.cpp
    enum.cpp
    event.cpp
    field.cpp
    folder.cpp
    fragment.cpp
    game.cpp
    group.cpp
    id.cpp
    image.cpp
    leaf.cpp
    meta.cpp
    mithril.cpp
    place.cpp
    quill.cpp
    root.cpp
    tag.cpp
    thing.cpp
    user.cpp
    value.cpp

    html/XEntry.cpp
    web/web_unity.cpp
    wksp/Workspace.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
    PRIVATE
        asio
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -DDEV_BUILD=1
        "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
        "-DYQ_SHARE_DIRS=\"${CMAKE_SOURCE_DIR}/share\"" 
)

LinkTest(${PROJECT_NAME})

