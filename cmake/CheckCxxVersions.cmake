
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "12.0.0")
        message(FATAL_ERROR "GCC version is obsolete, upgrade.")
    endif()
    SET(GCC ON)
else()
    SET(GCC OFF)
endif()

