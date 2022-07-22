################################################################################
##
##  YOUR QUILL
##
################################################################################

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
    
    ./atom/atom.cpp
    
    ./attr/attr.cpp
    
    ./bit/bit_unity.cpp
    
    ./c++/NaN.hpp
    
    
    ./db/db.cpp
    
    ./enum/enum_unity.cpp

    ./file/FileSpec.cpp
    ./file/file.cpp
    
    ./image/image.cpp
    
    ./io/DirWatcher.cpp
    ./io/DirWatcher.hpp
    
    ./ipc/ipc_unity.cpp

    ./leaf/leaf.cpp

    #./log/LogFile.cpp
    #./log/LogFile.hpp
    
    ./meta/MetaHtml.cpp
    ./meta/MetaHtml.hpp
    

    ./org/orgsys.cpp

    ./rule/rule.cpp

    
    ./srv/srv_unity.cpp
    ./srv/Notifier.hpp
    ./srv/NotifyAdapters.hpp
    ./srv/Stage2.hpp
    ./srv/Stage3.hpp
    ./srv/Stage4.hpp
    
    ./util/Markdown.cpp
    ./util/Markdown.hpp
    

    ./web/web_unity.cpp
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
)


IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yq 
    PUBLIC 
        yqtoolbox
	yqconfig
)

LinkTest(yq)

