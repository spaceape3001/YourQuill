set(stdquill_SOURCES 
    ./pages.cpp
    ./variables.cpp

    ./atom_sys.cpp
    ./common.cpp
    ./developer.cpp
    ./file_sys.cpp
    ./leaf_sys.cpp
    ./markdown.cpp
    ./tag_sys.cpp
    ./user.cpp
    ./workspace.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
#target_link_libraries( stdquill yq yqdb )

