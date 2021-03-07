add_executable(yserver
    yCommon.hpp
    yUpdater.cpp
    
    uClass.cpp
    uLeaf.cpp

    pAtomSys.cpp
    pBrowser.cpp
    pDeveloper.cpp
    pFileSys.cpp
    pWkspClass.cpp
    pWorkspace.cpp
    
    server.cpp
)
target_link_libraries(yserver yqdb yqmeta yqsrv yqutil Qt::Core  Qt::Gui Qt::Svg QtWebApp)

# target_compile_definitions(serverYQ PRIVATE SCANNER_EXE)

