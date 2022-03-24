set(stdpages_SOURCES 
    ./hare.cpp
    
    ./developer.cpp
    ./workspace.cpp
)

add_library( stdpages SHARED ${stdpages_SOURCES} )
#target_link_libraries( developer yq)

