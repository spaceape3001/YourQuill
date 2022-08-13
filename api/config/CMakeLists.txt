################################################################################
##
##  YOUR QUILL
##
################################################################################

add_library(yqconfig SHARED ./DirConfig.cpp ./DirConfig.hpp)

target_compile_definitions(yqconfig PRIVATE
    "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
    "-DYQ_SHARE_DIR=\"${YOUR_QUILL_KERNEL}/share\"" 
    "-DYQ_DATA_DIR=\"${CMAKE_SOURCE_DIR}/data\"" 
)

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
    target_link_libraries(yqconfig PRIVATE ${gcc-fslib})
ENDIF()
LinkTest(yqconfig)

