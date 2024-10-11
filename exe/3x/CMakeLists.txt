################################################################################
##
##  YOUR QUILL
##
################################################################################

#   3X is meant to be a fast command line processor for graphics processing
#   (STL generation, SVG generation, etc)  Might split into separate executables

add_executable(3x main.cpp)

target_link_libraries(3x PRIVATE yq_doodler)

