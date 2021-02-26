set(db_sources
    ./Access.hpp
    ./AssertDeny.hpp
    ./AtomSys.cpp
    ./AtomSys.hpp
    ./Attribute.hpp
    ./Attribute.cpp
    ./Cache.hpp
    ./CalSystem.hpp
    ./Change.hpp
    ./ClassFile.cpp
    ./ClassFile.hpp
    ./Context.cpp
    ./Context.hpp
    ./Copyright.cpp
    ./Copyright.hpp
    ./DataRole.hpp
    ./Enums.cpp
    ./Enums.hpp
    ./FileStrings.hpp
    ./FileSys.cpp
    ./FileSys.hpp
    ./FolderFile.cpp
    ./FolderFile.hpp
    ./Format.hpp
    ./Graph.cpp
    ./Graph.hpp
    ./GraphBuilder.cpp
    ./GraphBuilder.hpp
    ./Image.cpp
    ./Image.hpp
    ./IProvider.hpp
    ./Leaf.cpp
    ./Leaf.hpp
    ./LeafFile.cpp
    ./LeafFile.hpp
    ./Linkage.hpp
    ./Match.hpp
    ./Multiplicity.hpp
    ./NKI.hpp
    ./Policy.cpp
    ./Policy.hpp
    ./QuillData.hpp
    ./QuillFile.cpp
    ./QuillFile.hpp
    ./Restriction.hpp
    ./Root.hpp
    ./RootFwd.hpp
    ./ShareDir.cpp
    ./ShareDir.hpp
    ./SizeDesc.hpp
    ./Sorted.hpp
    ./StdFile.cpp
    ./StdFile.hpp
    ./Tag.cpp
    ./Tag.hpp
    ./TagFile.cpp
    ./TagFile.hpp
    ./Update.hpp
    ./Vcs.hpp
    ./Workspace.cpp
    ./Workspace.hpp
)

add_library(yqdb SHARED ${db_sources})

target_compile_definitions(yqdb PRIVATE "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" "-DYQ_SHARE_DIR=\"${CMAKE_SOURCE_DIR}/share\"" )

target_link_libraries(yqdb 
    PUBLIC 
        yqmeta
        yqutil
)



