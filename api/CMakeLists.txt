################################################################################
##
##  YOUR QUILL
##
################################################################################

project(mithril)

add_library(${PROJECT_NAME} SHARED
    mithril/atom.cpp
    mithril/attribute.cpp
    mithril/bit.cpp
    mithril/book.cpp
    mithril/category.cpp
    mithril/character.cpp
    mithril/class.cpp
    mithril/directories.cpp
    mithril/directory.cpp
    mithril/document.cpp
    mithril/enum.cpp
    mithril/event.cpp
    mithril/field.cpp
    mithril/folder.cpp
    mithril/fragment.cpp
    mithril/group.cpp
    mithril/image.cpp
    mithril/leaf.cpp
    mithril/meta.cpp
    mithril/mithril.cpp
    mithril/place.cpp
    mithril/quill.cpp
    mithril/root.cpp
    mithril/tag.cpp
    mithril/thing.cpp
    mithril/user.cpp
    mithril/value.cpp

    mithril/html/XEntry.cpp
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

