set(stdpages_SOURCES 
    ./hare.cpp
    
    ./developer.cpp
    ./dev_filesys.cpp
    ./dev_sql.cpp
    ./dev_wksp.cpp
    ./workspace.cpp
)

add_library( stdpages SHARED ${stdpages_SOURCES} )
#target_link_libraries( developer yq)

