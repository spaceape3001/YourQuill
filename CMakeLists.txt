################################################################################
##
##  YOUR QUILL
##
################################################################################

cmake_minimum_required(VERSION 3.26.0)
project(YourQuill VERSION 0.0.0.3 LANGUAGES CXX)

set(YOUR_QUILL_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(YQ_UNIVERSE_BUILD_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(YQ_UNIVERSE_SHARE_DIR ${CMAKE_CURRENT_LIST_DIR}/share)
include(local.cmake OPTIONAL)
include(configure.cmake)


if(PROJECT_IS_TOP_LEVEL)
    if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/yq-toolbox)
        message(FATAL_ERROR "YQ toolbox must be present! (use a symbolic link or junction)")
    endif()
    include(yq-toolbox/standard.cmake)
endif()

find_package(Graphviz)
find_package(ImageMagick COMPONENTS Magick++)

if(Build_Vulkan)
    vulqan_data(data)
endif()

YQRepo()
