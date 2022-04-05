#   Some of the major libraries will be combined into a single "unity-style" library for
#   cohesiveness (and the ability to cross-link).  Other libraries will be added independently.
#
#   Files should be listed alphabetically

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(yq_test_source ./test.cpp)
endif()

set(yq_sources
    ${yq_test_source}

    
    #   Disabled until needed
    #./ZipFile.cpp
    #./ZipReader.hpp
    #./ZipWriter.hpp
    
    ./library.cpp
    ./preamble.hpp

    ./algo/Compare.hpp
    ./algo/DiffEngine.hpp
    ./algo/Random.cpp
    ./algo/Random.hpp
    
    ./app/app_unity.cpp
    ./app/CmdArgs.hpp
    ./app/DelayInit.hpp
    ./app/PidFile.hpp
    ./app/Plugins.hpp
    
    ./atom/atom_unity.cpp
    ./atom/ClassData.hpp
    ./atom/ClassFile.hpp

    ./bit/bit_unity.cpp
    ./bit/Context.hpp
    ./bit/Copyright.hpp
    ./bit/KeyValue.cpp
    ./bit/KeyValue.hpp
    # ./bit/NKI.hpp
    ./bit/Policy.hpp

    ./c++/c++_unity.cpp
    ./c++/NaN.hpp
    ./c++/Safety.hpp
    ./c++/TypeTraits.hpp

    ./collection/Array1.hpp
    ./collection/Array2.hpp
    ./collection/collection_unity.cpp
    ./collection/Deque.hpp
    ./collection/EnumMap.hpp
    ./collection/Hash.hpp
    ./collection/List.hpp
    ./collection/Map.hpp
    ./collection/MultiMap.hpp
    ./collection/Set.hpp
    ./collection/Stack.hpp
    ./collection/Vector.hpp
    
    ./enum/enum_unity.cpp
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

    ./file/file_cdb.cpp
    ./file/file_unity.cpp
    ./file/file_watch.cpp
    ./file/cdb.hpp
    ./file/AbstractFile.hpp
    ./file/DirWatcher.hpp
    ./file/FileUtils.hpp
    ./file/StdFile.hpp
    ./file/XmlFile.hpp
    ./file/XmlFwd.hpp
    ./file/XmlUtils.hpp
    
    ./http/http_unity.cpp
    ./http/HttpData.hpp
    ./http/HttpParser.hpp
    ./http/HttpRequest.hpp
    
    ./image/Image.hpp
    
    ./ipc/ipc_unity.cpp
    ./ipc/ipcBuffer.hpp
    ./ipc/ipcPackets.hpp
    ./ipc/ipcSocket.hpp
    
    ./leaf/leaf_unity.cpp
    ./leaf/LeafFile.hpp

    #./log/LogFile.cpp
    #./log/LogFile.hpp
    ./log/Logging.cpp
    ./log/Logging.hpp
    
    ./meta/meta_unity.cpp
    ./meta/ArgInfo.hpp
    ./meta/Binder.hpp
    ./meta/CompoundInfo.hpp
    ./meta/DataBlock.hpp
    ./meta/Global.hpp
    ./meta/Init.hpp
    ./meta/Meta.hpp
    ./meta/MethodInfo.hpp
    ./meta/ObjectInfo.hpp
    ./meta/PropertyInfo.hpp
    ./meta/StdTypes.cpp
    ./meta/TypeInfo.hpp
    
    #./net/Curl.cpp
    #./net/Curl.hpp
    ./net/net_unity.cpp
    ./net/Http.hpp
    ./net/Url.hpp
    ./net/VersionSpec.hpp

    #./proc/Execute.cpp
    #./proc/Execute.hpp
    
    ./sql/sql_unity.cpp
    ./sql/SqlLite.hpp
    ./sql/SqlLogging.hpp
    ./sql/SqlQuery.hpp
    #./sql/SqlUtils.hpp
    
    ./srv/srv_unity.cpp
    ./srv/Notifier.hpp
    ./srv/NotifyAdapters.hpp
    ./srv/Stage2.hpp
    ./srv/Stage3.hpp
    ./srv/Stage4.hpp

    ./stream/stream_unity.cpp
    ./stream/Bytes.hpp
    ./stream/File.hpp
    ./stream/LineCol.hpp
    ./stream/OSFile.hpp
    ./stream/Ops.hpp
    
    ./tag/tag_unity.cpp
    ./tag/TagData.hpp
    ./tag/TagFile.hpp
    
    ./text/Comma.hpp
    ./text/Format.hpp
    ./text/Utils.cpp
    ./text/Utils.hpp
    
    ./type/type_unity.cpp
    ./type/ByteArray.hpp
    ./type/Enum.hpp
    ./type/Flag.hpp
    ./type/IntRange.hpp
    ./type/Ref.hpp
    ./type/Result.hpp
    ./type/Span.hpp
    ./type/Touched.hpp
    ./type/Variant.cpp

    ./web/web_unity.cpp
    ./web/WebAdapters.hpp
    ./web/WebHtml.hpp
    ./web/WebPage.hpp
    ./web/WebVariable.hpp

    ./wksp/CacheSQ.cpp
    ./wksp/CacheSQ.hpp
    ./wksp/QuillData.hpp
    ./wksp/QuillFile.cpp
    ./wksp/QuillFile.hpp
    ./wksp/RootFwd.hpp
    ./wksp/Root.hpp
    ./wksp/Workspace.cpp
    ./wksp/Workspace.hpp
)

add_library(yq SHARED ${yq_sources})
target_compile_definitions(yq PRIVATE 
    YQ__CORE__OVERRIDE 
    "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
    "-DYQ_SHARE_DIR=\"${CMAKE_SOURCE_DIR}/share\"" 
)


IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yq 
    PUBLIC 
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        ${GCC_FSLIB}
        ${SQLite3_LIBRARY}
)


