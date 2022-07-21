################################################################################
##
##  YOUR QUILL
##
################################################################################

cmake_minimum_required(VERSION 3.23.0)
project(YourQuill VERSION 0.0.0.3)
include(yq-utility/YourQuill.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
add_subdirectory(yq-utility)


# find_package(CURL)
# find_package(Log4cpp REQUIRED)
# find_package(TBB REQUIRED)
find_package(Graphviz)
find_package(ImageMagick COMPONENTS Magick++)

# find_package(libzip REQUIRED)
#   Qt's got issues, want to remove it....

set(VULKAN TRUE)

set(VULKAN_DIR $ENV{VULKAN_SDK})

    # for the "play"
find_package(Vulkan REQUIRED)
message("Vulkan should be in " $ENV{VULKAN_SDK})

find_package(GLFW3)


#   WARNING DEFINITIONS
add_definitions(-DASIO_STANDALONE)

if(Developer_Build)
    add_definitions(-DDEV_BUILD=1)
endif()

if(ENABLE_GLM)
    add_definitions(-DENABLE_GLM=1)
endif()

if(Feature_ApiDocumentation)
    find_package(Doxygen REQUIRED)
    if(DOXYGEN_FOUND)
    
    endif()
endif()





include_directories(${LOG4CPP_INCLUDE_DIR})
if(EXISTS apix)
    #   apix is intended as a local experimental API
    #include_directories(apix)
endif()

include_directories(api)
include_directories(${CMAKE_BINARY_DIR}/api)
include_directories(3rd/asio/asio/include)
include_directories(3rd/nlohmann/include)

if(Feature_PrecompiledHeaders)
    # SET(PRECOMPILE_HEADERS util/c++/stdlibs.hpp)
endif()

    #   We add support for "experimental" directories, either "s->x" or x appended.
    #   These won't be added to the main git, but allow for local experimentation

add_subdirectory(3rd)
if(Feature_Experimental AND EXISTS 3rdx)
    add_subdirectory(3rdx)
endif()
set(CMAKE_DEBUG_POSTFIX D)
#set(CMAKE_RELEASE_POSTFIX YQ)

add_definitions(-Werror)

add_definitions(-DGLFW_INCLUDE_VULKAN=1)

#add_subdirectory(res)
add_subdirectory(api)
#if(Feature_Experimental AND EXISTS apix)
    #add_subdirectory(apix)
#endif()
add_subdirectory(plugins)
if(Feature_Experimental AND EXISTS pluginx)
    #add_subdirectory(pluginx)
endif()
if(Feature_UnitTests)
    add_subdirectory(tests)
    if(Feature_Experimental AND EXISTS testx)
        #add_subdirectory(testx)
    endif()
endif()
add_subdirectory(tools)
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
add_subdirectory(examples)


