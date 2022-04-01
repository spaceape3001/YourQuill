set(stdpages_SOURCES 
    ./hare.cpp
    
    ./developer.cpp
    ./dev_sql.cpp
    ./workspace.cpp
)

add_library( stdpages SHARED ${stdpages_SOURCES} )
#target_link_libraries( developer yq)

