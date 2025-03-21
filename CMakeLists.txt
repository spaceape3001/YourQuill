################################################################################
##
##  YOUR QUILL
##
################################################################################

cmake_minimum_required(VERSION 3.26.0)
project(YourQuill VERSION 0.0.0.3 LANGUAGES CXX)


if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/yq-toolbox)
    message(FATAL_ERROR "YQ toolbox must be present! (use a symbolic link or junction)")
endif()

#   User features
option(Feature_BuildTime    "Print out build times" ON)

option(Feature_Doxygen      "Build doxygen documentation" OFF)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/yq-doodle)
    option(Feature_Doodle   "Enable the doodle API and features" ON)
    set(Build_Doodle ${Feature_Doodle})
else()
    set(Build_Doodle OFF)
endif()

option(Feature_Experimental "Enable Experimental Features" ON)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/yq-novel)
    option(Feature_Novel   "Enable the novel API and features" ON)
    set(Build_Novel ${Feature_Novel})
else()
    set(Build_Novel OFF)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/qt-utilities)
    option(Feature_Qt6      "Enable Qt6 (API and Apps)" ON)
    set(Build_Qt6 ${Feature_Qt6})
else()
    set(Build_Qt6 OFF)
endif()

option(Feature_SDK          "Enable SDK (Apps)" ON)
option(Feature_UnitTests    "Enable Unit Tests" ON)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/yq-vulqan)
    option(Feature_Vulkan   "Enable Vulkan (API and Apps)" OFF)
    set(Build_Vulkan ${Feature_Vulkan})
else()
    set(Build_Vulkan OFF)
endif()


# option(Feature_WebServer    "Enable Web Server" ON)

#   Translate features to build flags
#   (doing this so features/build can be decoupled, if necessary

set(Build_ASIO          ON)
set(Build_CURL          ON)
set(Build_Doxygen       ${Feature_Doxygen})
set(Build_GLM           ${Feature_Vulkan})
set(Build_Qt6           ${Feature_Qt6})
set(Build_SDK           ${Feature_SDK})
set(Build_SQL           ON)
set(Build_Timing        ${Feature_BuildTime})
set(Build_UnitTests     ${Feature_UnitTests})
set(Build_Vulkan        ${Feature_Vulkan})


include(yq-toolbox/configure.cmake)
add_subdirectory(yq-toolbox)

if(Build_Vulkan)
    include(yq-vulqan/configure.cmake)
    add_subdirectory(yq-vulqan)
endif()

if(Build_Qt6)
    add_subdirectory(qt-utilities)
endif()

if(Build_Doodle)
    add_subdirectory(yq-doodle)
endif()

if(Build_Novel)
    include(yq-vulqan/configure.cmake)
    add_subdirectory(yq-vulqan)
endif()

# add_subdirectory(3rd)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Graphviz)
find_package(ImageMagick COMPONENTS Magick++)

# find_package(libzip REQUIRED)
#   Qt's got issues, want to remove it....

    #   We add support for "experimental" directories, either "s->x" or x appended.
    #   These won't be added to the main git, but allow for local experimentation

include_directories(api)
    
if(Build_Qt6)
    add_subdirectory(res)
endif()

add_subdirectory(api)
add_subdirectory(plugins)
if(Build_UnitTests)
    add_subdirectory(test)
endif()
add_subdirectory(tool)
add_subdirectory(exe)



