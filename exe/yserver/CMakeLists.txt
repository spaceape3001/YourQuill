add_executable(yserver
    ./yCommon.hpp
    ./yUpdater.cpp
    
    ./update/uAtom.cpp
    ./update/uAtom.hpp
    ./update/uAttribute.cpp
    ./update/uAttribute.hpp
    ./update/uClass.cpp
    ./update/uClass.hpp
    ./update/uField.cpp
    ./update/uField.hpp
    ./update/uFwd.hpp
    ./update/uImage.cpp
    ./update/uImage.hpp
    ./update/uLeaf.cpp
    ./update/uLeaf.hpp
    ./update/uTag.cpp
    ./update/uTag.hpp

    ./page/pAtomSys.cpp
    ./page/pBrowser.cpp
    ./page/pDeveloper.cpp
    ./page/pFileSys.cpp
    ./page/pWkspClass.cpp
    ./page/pWorkspace.cpp
    
    ./server.cpp
)
target_link_libraries(yserver yqdb yqmeta yqsrv yqutil Qt::Core  Qt::Gui Qt::Svg QtWebApp)
target_include_directories(yserver PRIVATE .)

# target_compile_definitions(serverYQ PRIVATE SCANNER_EXE)

