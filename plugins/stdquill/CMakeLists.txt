set(stdquill_SOURCES 
    ./hare.cpp
    
    ./developer.cpp
    ./dev_sql.cpp
    ./dev_wksp.cpp
    
    ./atom_sys.cpp
    ./file_sys.cpp
    ./leaf_sys.cpp
    ./tag_sys.cpp
    ./workspace.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
#target_link_libraries( stdquill yq )

