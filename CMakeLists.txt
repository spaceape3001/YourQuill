################################################################################
##
##  YOUR QUILL
##
################################################################################

cmake_minimum_required(VERSION 3.26.0)
project(YourQuill VERSION 0.0.0.3 LANGUAGES CXX)

option(Feature_Qt6 "Deploy Qt6 (API and Apps)" ON)

include(yq-toolbox/configure.cmake)
add_subdirectory(yq-toolbox)
add_subdirectory(yq-vulqan)
if(Feature_Qt6)
    add_definitions(-DYQ_USE_QT)
    add_subdirectory(qt-utilities)
endif()
add_subdirectory(3rd)


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

option(Feature_Experimental "Enable Experimental Features" OFF)



find_package(Graphviz)
find_package(ImageMagick COMPONENTS Magick++)

# find_package(libzip REQUIRED)
#   Qt's got issues, want to remove it....


#   WARNING DEFINITIONS
add_definitions(-DASIO_STANDALONE)

if(Developer_Build)
    add_definitions(-DDEV_BUILD=1)
endif()

if(Feature_ApiDocumentation)
    find_package(Doxygen REQUIRED)
    if(DOXYGEN_FOUND)
    
    endif()
endif()



    #   We add support for "experimental" directories, either "s->x" or x appended.
    #   These won't be added to the main git, but allow for local experimentation

#set(CMAKE_RELEASE_POSTFIX YQ)

include_directories(api)


add_subdirectory(api)
add_subdirectory(plugins)
add_subdirectory(test)
add_subdirectory(tool)
if(Feature_Experimental AND EXISTS toolx)
    #add_subdirectory(toolx)
endif()
add_subdirectory(exe)
if(Feature_Experimental AND EXISTS exex)
    add_subdirectory(exex)
endif()
if(Feature_Experimental AND EXISTS play)
    add_subdirectory(play)
endif()



