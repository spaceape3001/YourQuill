add_executable(serverYQ
    main.cpp
    
    #yCommon.cpp
    #yCommon.hpp
    #yDatabase.cpp
    #yDatabase.hpp
    #yImporter.hpp
    #yLogger.cpp
    #yLogger.hpp
    #yNetWriter.cpp
    #yNetWriter.hpp
    #yPage.cpp
    #yPage.hpp
    #yScanner.hpp
    #yScanner.cpp
    #yUpdater.cpp
    
    #uClass.cpp
    #uLeaf.cpp

    #pAtomSys.cpp
    #pBrowser.cpp
    #pDeveloper.cpp
    #pFileSys.cpp
    #pWkspClass.cpp
    #pWorkspace.cpp

    #db.cpp
)
target_link_libraries(serverYQ yq Qt::Core  Qt::Gui Qt::Svg QtWebApp)

# target_compile_definitions(serverYQ PRIVATE SCANNER_EXE)

