cmake_minimum_required(VERSION 3.20.0)      # Modern CMake please
project(YourQuill VERSION 0.0.0.2)

include(cmake/CheckCxxVersions.cmake)

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
        # Mixing source and binary output is asking for trouble, best to avoid it
    message(SEND_ERROR "Building into the source directory is NOT recommended, choose another destination.")
endif()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    set(CMAKE_GENERATOR "Unix Makefiles")
    get_filename_component(CMAKE_BUILD_TYPE ${CMAKE_BINARY_DIR} NAME)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_CONFIGURATION_TYPES Debug;Release)

set(CMAKE_CXX_STANDARD 23)                  # C++20 is the standard (may be bumped in the future)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(Feature_ApiDocumentation "Enable Doxygen API Generation?" ON)
option(Feature_UnitTests "Enable Unit Tests" ON)
set(ENABLE_GLM OFF)
option(Feature_BuildTime "Print out build times" ON)
option(Feature_Experimental "Enable Experimental Features" ON)
option(Feature_PrecompiledHeaders "Enable Precompiled Headers" ON)
option(Developer_Build "Set for a development build" ON) # Disable for release-release

message( "Build Type is " ${CMAKE_BUILD_TYPE})

find_package(CURL)
find_package(Log4cpp REQUIRED)
find_package(TBB REQUIRED)
find_package(Graphviz)
find_package(SQLite3 REQUIRED)
find_package(ImageMagick COMPONENTS Magick++)

# find_package(libzip REQUIRED)
#   Qt's got issues, want to remove it....

set(VULKAN TRUE)

set(VULKAN_DIR $ENV{VULKAN_SDK})

    # for the "play"
find_package(Vulkan REQUIRED)
message("Vulkan should be in " $ENV{VULKAN_SDK})

find_package(GLFW3)

include(cmake/GroupFiles.cmake)
include(cmake/LinkTest.cmake)

#   WARNING DEFINITIONS
add_definitions(-Wfatal-errors -Wno-error=unused-function -Wno-error=unused-variable -Wall -fno-strict-aliasing -D_GNU_SOURCE -fdiagnostics-color -fdiagnostics-show-template-tree -fno-elide-type -fdiagnostics-path-format=inline-events -fdiagnostics-generate-patch -DASIO_STANDALONE)

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

enable_testing()

if(Feature_BuildTime)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
endif()


include_directories(${TBB_INCLUDE_DIR})
include_directories(${LOG4CPP_INCLUDE_DIR})
if(EXISTS apix)
    #   apix is intended as a local experimental API
    #include_directories(apix)
endif()
include_directories(api)
include_directories(${CMAKE_BINARY_DIR}/api)
include_directories(3rd/rapidxml-1.13)
include_directories(3rd/asio/asio/include)
include_directories(3rd/nlohmann/include)
include_directories(3rd/pstreams-1.0.3)
include_directories(3rd/imgui)

if(Feature_PrecompiledHeaders)
    SET(PRECOMPILE_HEADERS util/c++/stdlibs.hpp)
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
    include_directories(3rd/ut/include)
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


