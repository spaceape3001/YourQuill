################################################################################
##
##  YOUR QUILL
##
################################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin/plugin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugin)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_subdirectory(stdquill)
