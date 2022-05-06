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
    
    ./preamble.hpp
    

    ./algo/Compare.hpp
    ./algo/DiffEngine.hpp
    ./algo/Random.cpp
    ./algo/Random.hpp
    
    ./app/app_unity.cpp
    ./app/BuildInfo.hpp
    ./app/BuildInfo.ipp
    ./app/CmdArgs.hpp
    ./app/CmdArgs.ipp
    ./app/DelayInit.hpp
    ./app/PidFile.hpp
    ./app/Plugins.hpp
    ./app/Plugins.ipp
    ./app/ThreadId.hpp
    ./app/ThreadId.ipp
    
    ./atom/atom.cpp
    
    ./attr/attr.cpp
    
    ./bit/bit_unity.cpp
    
    ./c++/c++_unity.cpp
    ./c++/Concat.hpp
    ./c++/DbgRel.hpp
    ./c++/NaN.hpp
    ./c++/Safety.hpp
    
    ./c++/trait/always_false.hpp
    ./c++/trait/always_true.hpp
    ./c++/trait/can_two_add.hpp
    ./c++/trait/can_two_divide.hpp
    ./c++/trait/can_two_multiply.hpp
    ./c++/trait/can_two_subtract.hpp
    ./c++/trait/has_addition.hpp
    ./c++/trait/has_equality.hpp
    ./c++/trait/has_inequality.hpp
    ./c++/trait/has_less.hpp
    ./c++/trait/has_subtraction.hpp
    ./c++/trait/is_template.hpp
    ./c++/trait/not_copyable.hpp
    ./c++/trait/not_moveable.hpp
    ./c++/trait/sink.hpp

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
    
    ./db/db.cpp
    
    ./enum/enum_unity.cpp

    ./file/file.cpp
    
    ./io/file_utils.hpp
    ./io/file_unity.cpp
    ./io/file_watch.cpp
    ./io/AbstractFile.hpp
    ./io/DirWatcher.hpp
    ./io/XmlFile.hpp
    ./io/XmlFwd.hpp
    ./io/XmlUtils.hpp
    
    
    ./ipc/ipc_unity.cpp
    ./ipc/ipcBuffer.hpp
    ./ipc/ipcPackets.hpp
    ./ipc/ipcSocket.hpp

    ./leaf/leaf.cpp

    #./log/LogFile.cpp
    #./log/LogFile.hpp
    ./log/Logging.cpp
    ./log/Logging.hpp
    
    ./meta/meta_unity.cpp
    ./meta/ArgInfo.hpp
    ./meta/CompoundInfo.hpp
    ./meta/DataBlock.hpp
    ./meta/Global.hpp
    ./meta/InfoBinder.hpp
    ./meta/Init.hpp
    ./meta/Meta.hpp
    ./meta/MetaRepo.hpp
    ./meta/MetaRepo.ipp
    ./meta/MetaWriter.hpp
    ./meta/MetaWriter.ipp
    ./meta/MethodInfo.hpp
    ./meta/ObjectInfo.hpp
    ./meta/PropertyInfo.hpp
    ./meta/ReservedIDs.hpp
    ./meta/StdTypes.cpp
    ./meta/TypeInfo.hpp
    
    #./net/Curl.cpp
    #./net/Curl.hpp
    ./net/net_unity.cpp
    ./net/Http.hpp
    ./net/Url.hpp
    ./net/VersionSpec.hpp

    ./org/orgsys.cpp

    #./proc/Execute.cpp
    #./proc/Execute.hpp
    
    ./sql/SqlError.hpp
    ./sql/SqlError.ipp
    ./sql/SqlLastId.hpp
    ./sql/SqlLastId.ipp
    ./sql/SqlLite.hpp
    ./sql/SqlLite.ipp
    ./sql/SqlLogging.hpp
    ./sql/SqlQuery.hpp
    ./sql/SqlQuery.ipp
    ./sql/SqlUtils.hpp
    ./sql/SqlUtils.ipp
    ./sql/sql_unity.cpp
    
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
    
    
    ./text/Comma.hpp
    ./text/Encode64.cpp
    ./text/Encode64.hpp
    ./text/Format.hpp
    ./text/IgCase.hpp
    ./text/IgCase.ipp
    ./text/Markdown.cpp
    ./text/Markdown.hpp
    ./text/RevIgCase.hpp
    ./text/RevIgCase.ipp
    ./text/text_utils.hpp
    ./text/text_utils.ipp
    ./text/text_unity.cpp
    
    ./type/type_unity.cpp
    
    ./type/Any.hpp
    ./type/ByteArray.hpp
    ./type/Enum.hpp
    ./type/FixedBuffer.hpp
    ./type/Flag.hpp
    ./type/IntRange.hpp
    ./type/Ref.hpp
    ./type/Result.hpp
    ./type/Span.hpp
    ./type/Touched.hpp

    ./web/web_unity.cpp
    ./web/HttpParser.hpp
    ./web/HttpParser.ipp
    ./web/JsonAdapter.hpp
    ./web/PageTemplate.hpp
    ./web/PageTemplate.ipp
    ./web/WebAdapters.hpp
    ./web/WebAdapters.ipp
    ./web/WebContext.hpp
    ./web/WebContext.ipp
    ./web/WebHtml.hpp
    ./web/WebHtml.ipp
    ./web/WebPage.hpp
    ./web/WebPage.ipp
    ./web/WebVariable.hpp
    ./web/WebVariable.ipp
    
    ./wksp/wksp.cpp
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

#   override for temp
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/garbage)
add_executable( link_test_yqlib linktest.cpp )
target_link_libraries( link_test_yqlib yq )


