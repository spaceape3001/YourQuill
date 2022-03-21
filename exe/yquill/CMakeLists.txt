add_executable(yquill
    #./yCommon.hpp
    #./yUpdater.cpp
    
    #./uAtom.ipp
    #./uAtom.hpp
    #./uAttribute.ipp
    #./uAttribute.hpp
    #./uClass.ipp
    #./uClass.hpp
    #./uField.ipp
    #./uField.hpp
    #./uFwd.hpp
    #./uImage.ipp
    #./uImage.hpp
    #./uLeaf.ipp
    #./uLeaf.hpp
    #./uTag.ipp
    #./uTag.hpp
    
    #./pages.cpp

    #./pAtomSys.ipp
    #./pBrowser.ipp
    #./pDeveloper.ipp
    #./pFileSys.ipp
    #./pWkspClass.ipp
    #./pWorkspace.ipp
    
    #./server.cpp
    
    ./yquill.cpp
)
#target_link_libraries(yquill yqdb yqmeta yqsrv yqutil Qt::Core  Qt::Gui Qt::Svg QtWebApp)
target_link_libraries(yquill yqdb yq)
target_include_directories(yquill PRIVATE .)


