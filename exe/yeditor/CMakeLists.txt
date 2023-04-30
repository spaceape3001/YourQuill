################################################################################
##
##  YOUR QUILL
##
################################################################################

SET(yeditor_sources ./yeditor.cpp)
add_executable(yeditor ${yeditor_sources})
target_link_libraries(yeditor mithril tachyon json)


