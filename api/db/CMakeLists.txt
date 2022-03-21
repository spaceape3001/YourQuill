set(db_sources
    #./Atom.hpp
    #./Atom.ipp
    #./Attribute.hpp
    #./Attribute.ipp
    #./Cache.hpp
    #./CacheFwd.hpp
    #./CacheUtil.hpp
    #./CacheUtil.ipp
    #./Class.hpp
    #./Class.ipp
    #./ClassFile.hpp
    #./ClassFile.ipp
    #./CommonDirs.hpp
    #./CommonDirs.ipp
    #./DBUnity.cpp
    #./DBUnity2.cpp
    #./Directory.hpp
    #./Document.hpp
    #./Field.hpp
    #./FileStrings.hpp
    #./FileSys.hpp
    #./FileSys.ipp
    #./FileUnity.cpp
    #./Folder.hpp
    #./FolderFile.hpp
    #./FolderFile.ipp
    #./Fragment.hpp
    #./Graph.hpp
    #./Graph.ipp
    #./GraphBuilder.hpp
    #./GraphBuilder.ipp
    #./Image.hpp
    #./Image.ipp
    #./Leaf.hpp
    #./Leaf.ipp
    #./LeafFile.hpp
    #./LeafFile.ipp
    #./Property.cpp
    #./Property.hpp
    #./QuillData.hpp
    #./QuillFile.hpp
    #./QuillFile.ipp
    #./Root.hpp
    #./RootFwd.hpp
    #./ShareDir.hpp
    #./ShareDir.ipp
    #./StdFile.hpp
    #./StdFile.ipp
    #./Tag.hpp
    #./Tag.ipp
    #./TagFile.hpp
    #./TagFile.ipp
    #./Wksp.cpp
    #./Wksp.hpp
    #./Workspace.hpp
    #./Workspace.ipp

    ./bit/Bits.cpp
    ./bit/Context.hpp
    ./bit/Copyright.hpp
    ./bit/KeyValue.cpp
    ./bit/KeyValue.hpp
    ./bit/NKI.hpp
    ./bit/Policy.hpp

    ./enum/Access.hpp
    ./enum/AssertDeny.hpp
    ./enum/AttrKind.hpp
    ./enum/CalSystem.hpp
    ./enum/Change.hpp
    ./enum/DataRole.hpp
    ./enum/Format.hpp
    ./enum/Linkage.hpp
    ./enum/Match.hpp
    ./enum/Multiplicity.hpp
    ./enum/Restriction.hpp
    ./enum/SizeDesc.hpp
    ./enum/Sorted.hpp
    ./enum/Update.hpp
    ./enum/Vcs.hpp

    ./enum/Enums.cpp
    
    ./wksp/QuillData.hpp
    ./wksp/QuillFile.cpp
    ./wksp/QuillFile.hpp
    ./wksp/RootFwd.hpp
    ./wksp/Root.hpp
    ./wksp/Workspace.cpp
    ./wksp/Workspace.hpp
)

add_library(yqdb SHARED ${db_sources})

target_compile_definitions(yqdb PRIVATE "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" "-DYQ_SHARE_DIR=\"${CMAKE_SOURCE_DIR}/share\"" )

target_link_libraries(yqdb 
    PUBLIC 
        yq
)



