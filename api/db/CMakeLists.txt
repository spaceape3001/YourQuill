set(db_sources
    ./AtomSys.cpp
    ./AtomSys.hpp
    ./Cache.hpp
    ./ClassFile.cpp
    ./ClassFile.hpp
    ./Enums.cpp
    ./Enums.hpp
    ./FileStrings.hpp
    ./FileSys.cpp
    ./FileSys.hpp
    ./FolderFile.cpp
    ./FolderFile.hpp
    ./Graph.cpp
    ./Graph.hpp
    ./GraphBuilder.cpp
    ./GraphBuilder.hpp
    ./Image.cpp
    ./Image.hpp
    ./Leaf.cpp
    ./Leaf.hpp
    ./LeafFile.cpp
    ./LeafFile.hpp
    ./QuillData.hpp
    ./QuillFile.cpp
    ./QuillFile.hpp
    ./Root.hpp
    ./RootFwd.hpp
    ./ShareDir.cpp
    ./ShareDir.hpp
    ./StdFile.cpp
    ./StdFile.hpp
    ./Tag.cpp
    ./Tag.hpp
    ./TagFile.cpp
    ./TagFile.hpp
    ./Workspace.cpp
    ./Workspace.hpp

    ./bit/Attribute.hpp
    ./bit/Attribute.cpp
    ./bit/Context.cpp
    ./bit/Context.hpp
    ./bit/Copyright.cpp
    ./bit/Copyright.hpp
    ./bit/NKI.hpp
    ./bit/Policy.cpp
    ./bit/Policy.hpp

    ./enum/Access.hpp
    ./enum/AssertDeny.hpp
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



