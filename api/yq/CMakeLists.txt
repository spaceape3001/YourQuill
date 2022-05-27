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
    
    ./app/app_unity.cpp
    ./app/BuildInfo.hpp
    ./app/BuildInfo.ipp
    ./app/PidFile.hpp
    ./app/Plugins.hpp
    ./app/Plugins.ipp
    
    ./atom/atom.cpp
    
    ./attr/attr.cpp
    
    ./bit/bit_unity.cpp
    
    ./c++/NaN.hpp
    
    
    ./db/db.cpp
    
    ./enum/enum_unity.cpp

    ./file/file.cpp
    
    ./image/image.cpp
    
    ./io/file_unity.cpp
    ./io/file_watch.cpp
    ./io/AbstractFile.hpp
    ./io/DirWatcher.hpp
    ./io/XmlFile.hpp
    
    ./ipc/ipc_unity.cpp
    ./ipc/ipcBuffer.hpp
    ./ipc/ipcPackets.hpp
    ./ipc/ipcSocket.hpp

    ./leaf/leaf.cpp

    #./log/LogFile.cpp
    #./log/LogFile.hpp
    
    ./meta/meta_unity.cpp
    ./meta/MetaHtml.cpp
    ./meta/StdTypes.cpp
    
    #./net/Curl.cpp
    #./net/Curl.hpp
    ./net/net_unity.cpp
    ./net/Http.hpp
    ./net/Url.hpp
    ./net/VersionSpec.hpp

    ./org/orgsys.cpp

    ./rule/rule.cpp

    
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
    
    
    ./text/Encode64.cpp
    ./text/Encode64.hpp
    ./text/Markdown.cpp
    ./text/Markdown.hpp
    
    ./type/type_unity.cpp
    
    ./type/Any.hpp

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
    
    ./wksp/Workspace.cpp
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
        yqbasic
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        ${GCC_FSLIB}
        ${SQLite3_LIBRARY}
)

LinkTest(yq)

