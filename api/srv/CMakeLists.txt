set(srv_sources
    #./ArgDecode.cpp
    #./ArgDecode.hpp
    ./Session.cpp
    ./Session.hpp
    ./SrvLogging.hpp
    ./TLSGlobals.cpp
    ./TLSGlobals.hpp
    ./TypedBytes.cpp
    ./TypedBytes.hpp
)

add_library(yqsrv SHARED ${srv_sources})
target_link_libraries(yqsrv PUBLIC yqdb QtWebApp)


