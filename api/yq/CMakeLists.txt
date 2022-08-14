################################################################################
##
##  YOUR QUILL
##
################################################################################

include(${YOUR_QUILL_EXTERNAL}/asio.cmake)

#   Some of the major libraries will be combined into a single "unity-style" library for
#   cohesiveness (and the ability to cross-link).  Other libraries will be added independently.
#
#   Files should be listed alphabetically

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(yq_test_source ./test.cpp)
endif()

set(yq_sources
    ${yq_test_source}

    
    #   Disabled until needed
    #./ZipFile.cpp
    #./ZipReader.hpp
    #./ZipWriter.hpp
    
    ./atom/atom.cpp
    
    ./attr/attr.cpp
    
    ./file/file.cpp
    
    ./image/image.cpp
    
    ./leaf/leaf.cpp

    #./log/LogFile.cpp
    #./log/LogFile.hpp

    ./org/orgsys.cpp

    
    ./wksp/wksp.cpp
)

add_library(yq SHARED ${yq_sources})
target_compile_definitions(yq PRIVATE 
    YQ__CORE__OVERRIDE 
)


IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yq 
    PUBLIC 
    yqtoolbox
    asio
    yqkernel
)

LinkTest(yq)

