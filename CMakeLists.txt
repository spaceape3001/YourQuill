cmake_minimum_required(VERSION 3.20.0)      # Modern CMake please
project(YourQuill VERSION 0.0.0.1)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "11.0.0")
        message(FATAL_ERROR "GCC version is obsolete, upgrade.")
    endif()
    SET(GCC ON)
else()
    SET(GCC OFF)
endif()

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
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(Feature_ApiDocumentation "Enable Doxygen API Generation?" ON)
option(Feature_QtGUI "Enable Qt GUI features?" OFF)
option(Feature_UnitTests "Enable Unit Tests" ON)
if(${Feature_QtGUI})
    set(ENABLE_QT ON)
endif()
set(ENABLE_GLM OFF)
option(Feature_BuildTime "Print out build times" ON)
option(Feature_Experimental "Enable Experimental Features" OFF)

message( "Build Type is " ${CMAKE_BUILD_TYPE})

find_package(CURL)
find_package(Log4cpp REQUIRED)
find_package(TBB REQUIRED)
find_package(Graphviz)
find_package(SQLite3)
# find_package(libzip REQUIRED)
#   Qt's got issues, want to remove it....
find_package(Qt5 REQUIRED Core Gui Network Sql Svg  Widgets WebEngine WebEngineWidgets)

# find_package(nghttp2)

if(Feature_Experimental AND EXISTS play)
    set(VULKAN TRUE)
endif()

if(VULKAN)
    set(VULKAN_DIR $ENV{VULKAN_SDK})

        # for the "play"
    find_package(Vulkan REQUIRED)
    message("Vulkan should be in " $ENV{VULKAN_SDK})

    find_package(GLFW3)
endif()

function(group_files)
        foreach(_source ${ARGN})
                get_filename_component(_source_path "${_source}" PATH)
                string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
                source_group("${_source_path_msvc}" FILES "${_source}")
        endforeach()
endfunction(group_files)

add_definitions(-Wfatal-errors -Wno-error=unused-function -Wall -fno-strict-aliasing -D_GNU_SOURCE)

add_definitions(-DENABLE_QT=1)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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

include_directories(3rd/QtWebApp-1.7.11/QtWebApp)
include_directories(${TBB_INCLUDE_DIR})
include_directories(${LOG4CPP_INCLUDE_DIR})
if(EXISTS apix)
    #   apix is intended as a local experimental API
    include_directories(apix)
endif()
include_directories(api)
include_directories(${CMAKE_BINARY_DIR}/api)
include_directories(3rd/rapidxml-1.13)
include_directories(3rd/asio-1.18.0/include)
include_directories(${nghttp2_INCLUDE_DIRS})

    #   We add support for "experimental" directories, either "s->x" or x appended.
    #   These won't be added to the main git, but allow for local experimentation

add_subdirectory(3rd)
if(Feature_Experimental AND EXISTS 3rdx)
    add_subdirectory(3rdx)
endif()
set(CMAKE_DEBUG_POSTFIX D)
#set(CMAKE_RELEASE_POSTFIX YQ)

add_definitions(-Werror)

add_subdirectory(res)
add_subdirectory(api)
if(Feature_Experimental AND EXISTS apix)
    add_subdirectory(apix)
endif()
add_subdirectory(plugins)
if(Feature_Experimental AND EXISTS pluginx)
    add_subdirectory(pluginx)
endif()
if(Feature_UnitTests)
    add_subdirectory(tests)
    if(Feature_Experimental AND EXISTS testx)
        add_subdirectory(testx)
    endif()
endif()
add_subdirectory(tools)
if(Feature_Experimental AND EXISTS toolx)
    add_subdirectory(toolx)
endif()
add_subdirectory(exe)
if(Feature_Experimental AND EXISTS exex)
    add_subdirectory(exex)
endif()
if(Feature_Experimental AND EXISTS play)
    add_subdirectory(play)
endif()


