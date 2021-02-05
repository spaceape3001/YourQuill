set(QtWebApp_SOURCES
    httpserver/httpconnectionhandler.cpp
    httpserver/httpconnectionhandler.h
    httpserver/httpconnectionhandlerpool.cpp
    httpserver/httpconnectionhandlerpool.h
    httpserver/httpcookie.cpp
    httpserver/httpcookie.h
    httpserver/httpglobal.cpp
    httpserver/httpglobal.h
    httpserver/httplistener.cpp
    httpserver/httplistener.h
    httpserver/httprequest.cpp
    httpserver/httprequest.h
    httpserver/httprequesthandler.cpp
    httpserver/httprequesthandler.h
    httpserver/httpresponse.cpp
    httpserver/httpresponse.h
    httpserver/httpserver.pri
    httpserver/httpsession.cpp
    httpserver/httpsession.h
    httpserver/httpsessionstore.cpp
    httpserver/httpsessionstore.h
    httpserver/staticfilecontroller.cpp
    httpserver/staticfilecontroller.h
)

add_library(QtWebApp SHARED ${QtWebApp_SOURCES})
set_target_properties(QtWebApp PROPERTIES VERSION ${QWAVer})

target_link_libraries(QtWebApp PUBLIC Qt5::Network)

