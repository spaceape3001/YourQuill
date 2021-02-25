add_executable(serverYQ
    yCommon.cpp
    yCommon.hpp
    yImporter.hpp
    yNetWriter.cpp
    yNetWriter.hpp
    yPage.cpp
    yPage.hpp
    yScanner.cpp
    yScanner.hpp
    yUpdater.cpp
    
    uClass.cpp
    uLeaf.cpp

    pAtomSys.cpp
    pBrowser.cpp
    pDeveloper.cpp
    pFileSys.cpp
    pWkspClass.cpp
    pWorkspace.cpp
    
    db.cpp
)
target_link_libraries(serverYQ yqdb yqmeta yqutil Qt::Core  Qt::Gui Qt::Svg QtWebApp)

# target_compile_definitions(serverYQ PRIVATE SCANNER_EXE)

