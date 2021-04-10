set(QtWebApp_SOURCES
    httpserver/httpconnectionhandler.ipp
    httpserver/httpconnectionhandler.h
    httpserver/httpconnectionhandlerpool.ipp
    httpserver/httpconnectionhandlerpool.h
    httpserver/httpcookie.ipp
    httpserver/httpcookie.h
    httpserver/httpglobal.ipp
    httpserver/httpglobal.h
    httpserver/httplistener.ipp
    httpserver/httplistener.h
    httpserver/httprequest.ipp
    httpserver/httprequest.h
    httpserver/httprequesthandler.ipp
    httpserver/httprequesthandler.h
    httpserver/httpresponse.ipp
    httpserver/httpresponse.h
    httpserver/httpsession.ipp
    httpserver/httpsession.h
    httpserver/httpsessionstore.ipp
    httpserver/httpsessionstore.h
    httpserver/httpunity.cpp
    httpserver/staticfilecontroller.ipp
    httpserver/staticfilecontroller.h
)

add_library(QtWebApp SHARED ${QtWebApp_SOURCES})
set_target_properties(QtWebApp PROPERTIES VERSION ${QWAVer})

target_link_libraries(QtWebApp PUBLIC Qt5::Network)

