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

#   SUPERSCRIPTS ⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹ ʰ ᵖ ⁿ ʳ ˢ ᵗ ᵘ ᵛ ʷ ˣ ʸ ᶻ
#   Numbers ① ② ③ ④

project(yq_doodler)

add_library(${PROJECT_NAME} SHARED

    Association.cpp
    Camera.cpp
    Constraint.cpp
    DObject.cpp
    Light.cpp
    Motion.cpp

    Py.cpp
    
    1D/Py1.cpp
    
    2D/Py2.cpp
    
    3D/Py3.cpp
    
    4D/Py4.cpp

    b3/grammar.cpp

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

