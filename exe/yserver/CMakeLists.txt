add_executable(yserver
    ./yCommon.hpp
    ./yUpdater.cpp
    
    ./uAtom.ipp
    ./uAtom.hpp
    ./uAttribute.ipp
    ./uAttribute.hpp
    ./uClass.ipp
    ./uClass.hpp
    ./uField.ipp
    ./uField.hpp
    ./uFwd.hpp
    ./uImage.ipp
    ./uImage.hpp
    ./uLeaf.ipp
    ./uLeaf.hpp
    ./uTag.ipp
    ./uTag.hpp
    
    ./pages.cpp

    ./pAtomSys.ipp
    ./pBrowser.ipp
    ./pDeveloper.ipp
    ./pFileSys.ipp
    ./pWkspClass.ipp
    ./pWorkspace.ipp
    
    ./server.cpp
)
target_link_libraries(yserver yqdb yqmeta yqsrv yqutil Qt::Core  Qt::Gui Qt::Svg QtWebApp)
target_include_directories(yserver PRIVATE .)

# target_compile_definitions(serverYQ PRIVATE SCANNER_EXE)

