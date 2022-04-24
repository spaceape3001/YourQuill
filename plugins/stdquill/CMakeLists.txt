set(stdquill_SOURCES 
    ./authentication.cpp
    ./imaging.cpp
    ./pages.cpp
    ./updater.cpp
    ./variables.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
target_include_directories( stdquill PRIVATE ${ImageMagick_INCLUDE_DIRS}) 
target_link_libraries( stdquill PUBLIC yq yqdb yqdtn ${ImageMagick_LIBRARIES})


#   override for temp
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/garbage)

add_executable( link_test_stdquill linktest.cpp )
target_link_libraries( link_test_stdquill stdquill )

