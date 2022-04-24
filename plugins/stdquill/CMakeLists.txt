set(stdquill_SOURCES 
    ./authentication.cpp
    ./imaging.cpp
    ./pages.cpp
    ./updater.cpp
    ./variables.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
target_include_directories( stdquill PRIVATE ${ImageMagick_INCLUDE_DIRS}) 
target_link_libraries( stdquill PRIVATE yq yqdb ${ImageMagick_LIBRARIES})

