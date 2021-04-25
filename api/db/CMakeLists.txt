set(db_sources
    ./Atom.hpp
    ./Atom.ipp
    ./Attribute.hpp
    ./Attribute.ipp
    ./Cache.hpp
    ./CacheFwd.hpp
    ./CacheUtil.hpp
    ./CacheUtil.ipp
    ./Class.hpp
    ./Class.ipp
    ./ClassFile.hpp
    ./ClassFile.ipp
    ./DBUnity.cpp
    ./Directory.hpp
    ./Document.hpp
    ./Enums.cpp
    ./Enums.hpp
    ./Field.hpp
    ./FileStrings.hpp
    ./FileSys.hpp
    ./FileSys.ipp
    ./FileUnity.cpp
    ./Folder.hpp
    ./FolderFile.hpp
    ./FolderFile.ipp
    ./Fragment.hpp
    ./Graph.hpp
    ./Graph.ipp
    ./GraphBuilder.hpp
    ./GraphBuilder.ipp
    ./Image.hpp
    ./Image.ipp
    ./Leaf.hpp
    ./Leaf.ipp
    ./LeafFile.hpp
    ./LeafFile.ipp
    ./Property.cpp
    ./Property.hpp
    ./QuillData.hpp
    ./QuillFile.hpp
    ./QuillFile.ipp
    ./Root.hpp
    ./RootFwd.hpp
    ./ShareDir.hpp
    ./ShareDir.ipp
    ./StdFile.hpp
    ./StdFile.ipp
    ./Tag.hpp
    ./Tag.ipp
    ./TagFile.hpp
    ./TagFile.ipp
    ./Wksp.cpp
    ./Wksp.hpp
     
    ./bit/BitUnity.cpp
    ./bit/Context.hpp
    ./bit/Context.ipp
    ./bit/Copyright.hpp
    ./bit/Copyright.ipp
    ./bit/KeyValue.hpp
    ./bit/KeyValue.ipp
    ./bit/NKI.hpp
    ./bit/Policy.hpp
    ./bit/Policy.ipp

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
)

add_library(yqdb SHARED ${db_sources})

target_compile_definitions(yqdb PRIVATE "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" "-DYQ_SHARE_DIR=\"${CMAKE_SOURCE_DIR}/share\"" )

target_link_libraries(yqdb 
    PUBLIC 
        yqmeta
        yqutil
)



