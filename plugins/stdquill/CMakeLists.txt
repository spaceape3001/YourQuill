set(stdquill_SOURCES 
    ./pages.cpp
    ./variables.cpp

    ./common.cpp
    ./developer.cpp
    ./tag_sys.cpp
    ./user.cpp
    ./workspace.cpp
)

add_library( stdquill SHARED ${stdquill_SOURCES} )
#target_link_libraries( stdquill yq yqdb )

