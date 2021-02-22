set(net_sources
    ./net/Session.cpp
    ./net/Session.hpp
)

add_library(yqnet SHARED ${net_sources})

target_link_libraries(yqnet 
    PUBLIC 
        yq
)


