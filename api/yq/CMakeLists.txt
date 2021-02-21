#   Some of the major libraries will be combined into a single "unity-style" library for
#   cohesiveness (and the ability to cross-link).  Other libraries will be added independently.
#
#   Files should be listed alphabetically

set(yq_sources
    ./Loggers.hpp
    
    ./api/AProvider.hpp
    
    ./bit/Attribute.hpp
    ./bit/Attribute.cpp
    ./bit/Context.cpp
    ./bit/Context.hpp
    ./bit/Copyright.cpp
    ./bit/Copyright.hpp
    ./bit/Policy.cpp
    ./bit/Policy.hpp
    
    ./cache/FileSys.cpp
    ./cache/FileSys.hpp
    ./cache/Graph.cpp
    ./cache/Graph.hpp
    ./cache/GraphBuilder.cpp
    ./cache/GraphBuilder.hpp
    ./cache/Image.cpp
    ./cache/Image.hpp
    ./cache/NKI.hpp

    ./enum/Access.hpp
    ./enum/AssertDeny.hpp
    ./enum/CalSystem.hpp
    ./enum/DataRole.hpp
    ./enum/Enums.cpp
    ./enum/Format.hpp
    ./enum/Linkage.hpp
    ./enum/Multiplicity.hpp
    ./enum/Restriction.hpp
    ./enum/SizeDesc.hpp
    ./enum/Sorted.hpp
    ./enum/Update.hpp
    ./enum/Vcs.hpp
    
    ./io/AbstractFile.cpp
    ./io/AbstractFile.hpp
    ./io/Curl.cpp
    ./io/Curl.hpp
    ./io/Execute.cpp
    ./io/Execute.hpp
    ./io/FileUtils.hpp
    ./io/Http.cpp
    ./io/Http.hpp
    ./io/Logging.cpp
    ./io/Logging.hpp
    ./io/ShareDir.cpp
    ./io/ShareDir.hpp
    ./io/StdFile.cpp
    ./io/StdFile.hpp
    ./io/Stream.hpp
    ./io/XmlFile.hpp
    ./io/XmlFwd.hpp
    ./io/ZipFile.cpp
    ./io/ZipReader.hpp
    ./io/ZipWriter.hpp

    ./meta/Global.hpp
    ./meta/GlobalImpl.hpp
    ./meta/InfoWriterImpl.hpp
    ./meta/Meta.cpp
    ./meta/MetaObject.hpp
    ./meta/MetaType.hpp
    ./meta/MetaValue.hpp
    ./meta/Object.cpp
    ./meta/Object.hpp
    ./meta/StdTypes.cpp
    ./meta/Variant.cpp
    ./meta/Variant.hpp

    ./sql/SqlQuery.cpp
    ./sql/SqlQuery.hpp
    ./sql/SqlUtils.cpp
    ./sql/SqlUtils.hpp

    ./util/Array2.hpp
    ./util/Collections.cpp
    ./util/Comma.hpp
    ./util/Compare.hpp
    ./util/DelayInit.cpp
    ./util/DelayInit.hpp
    ./util/Enum.cpp
    ./util/Enum.hpp
    ./util/EnumMap.hpp
    ./util/FileName.cpp
    ./util/FileName.hpp
    ./util/Flag.hpp
    ./util/Guarded.hpp
    ./util/Hash.hpp
    ./util/IntRange.hpp
    ./util/List.hpp
    ./util/Map.hpp
    ./util/MultiMap.hpp
    ./util/Ref.cpp
    ./util/Ref.hpp
    ./util/Safety.hpp
    ./util/Set.hpp
    ./util/Stack.hpp
    ./util/String.cpp
    ./util/String.hpp
    ./util/Utilities.cpp
    ./util/Utilities.hpp
    ./util/Vector.hpp
    
    ./wksp/QuillFile.cpp
    ./wksp/QuillFile.hpp
    ./wksp/Root.hpp
    ./wksp/RootFwd.hpp
    ./wksp/Workspace.cpp
    ./wksp/Workspace.hpp
)

add_library(yq SHARED ${yq_sources})
target_compile_definitions(yq PRIVATE YQ__CORE__OVERRIDE "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" "-DYQ_SHARE_DIR=\"${CMAKE_SOURCE_DIR}/share\"")

target_link_libraries(yq 
    PUBLIC 
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        Qt5::Core Qt5::Gui Qt5::Sql

)


