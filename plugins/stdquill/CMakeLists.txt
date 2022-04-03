set(stdquill_SOURCES 
    ./hare.cpp
    
    ./developer.cpp
    ./dev_filesys.cpp
    ./dev_sql.cpp
    ./dev_wksp.cpp
    ./workspace.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
#target_link_libraries( stdquill yq )

