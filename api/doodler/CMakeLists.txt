################################################################################
##
##  YOUR QUILL
##
################################################################################

#   Doodle is meant to be a fast sketchup capability, so rather than focusing
#   on lines/rectangles, we focus on objects, like chairs, rooms, windows, etc.
#
#   Two libraries... this one is about data, formats, algorithms.  Qt one is
#   about display widgets (for Qt)

project(doodler)

add_library(${PROJECT_NAME} SHARED
    doodle.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yq_core
        yq_math
        yq_xml
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
)

LinkTest(${PROJECT_NAME})

