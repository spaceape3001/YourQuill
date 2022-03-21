#   Some of the major libraries will be combined into a single "unity-style" library for
#   cohesiveness (and the ability to cross-link).  Other libraries will be added independently.
#
#   Files should be listed alphabetically

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(util_test_source ./test.cpp)
endif()

set(util_sources
    ${util_test_source}

    
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
    
    ./app/App.cpp
    ./app/CmdArgs.hpp
    ./app/DelayInit.hpp
    ./app/PidFile.hpp
    ./app/Plugins.hpp

    ./c++/C++.cpp
    ./c++/NaN.hpp
    ./c++/Safety.hpp
    ./c++/TypeTraits.hpp

    ./collection/Array1.hpp
    ./collection/Array2.hpp
    ./collection/Collections.cpp
    ./collection/Deque.hpp
    ./collection/EnumMap.hpp
    ./collection/Hash.hpp
    ./collection/List.hpp
    ./collection/Map.hpp
    ./collection/MultiMap.hpp
    ./collection/Set.hpp
    ./collection/Stack.hpp
    ./collection/Vector.hpp
    
    ./file/AbstractFile.hpp
    ./file/File.cpp
    ./file/FileUtils.hpp
    ./file/XmlFile.hpp
    ./file/XmlFwd.hpp
    ./file/XmlUtils.hpp
    
    #./log/LogFile.cpp
    #./log/LogFile.hpp
    ./log/Logging.cpp
    ./log/Logging.hpp
    
    ./meta/ArgInfo.hpp
    ./meta/Binder.hpp
    ./meta/CompoundInfo.hpp
    ./meta/DataBlock.hpp
    ./meta/Global.hpp
    ./meta/Init.hpp
    ./meta/Meta.cpp
    ./meta/Meta.hpp
    ./meta/MethodInfo.hpp
    ./meta/ObjectInfo.hpp
    ./meta/PropertyInfo.hpp
    ./meta/StdTypes.cpp
    ./meta/TypeInfo.hpp
    
    #./net/Curl.cpp
    #./net/Curl.hpp
    ./net/Http.hpp
    ./net/NetUnity.cpp
    ./net/Url.hpp
    ./net/VersionSpec.hpp

    #./proc/Execute.cpp
    #./proc/Execute.hpp
    
    #./sql/Sql.cpp
    #./sql/SqlLogging.hpp
    #./sql/SqlQuery.hpp
    #./sql/SqlUtils.hpp

    ./stream/Bytes.hpp
    ./stream/File.hpp
    ./stream/LineCol.hpp
    ./stream/OSFile.hpp
    ./stream/Ops.hpp
    ./stream/Stream.cpp
    
    ./text/Comma.hpp
    ./text/Format.hpp
    ./text/Utils.cpp
    ./text/Utils.hpp
    
    ./type/ByteArray.hpp
    ./type/Enum.hpp
    ./type/Flag.hpp
    ./type/IntRange.hpp
    ./type/Ref.hpp
    ./type/Result.hpp
    ./type/Span.hpp
    ./type/Touched.hpp
    ./type/Type.cpp
    ./type/Variant.cpp
)

add_library(yqutil SHARED ${util_sources})
target_compile_definitions(yqutil PRIVATE 
    YQ__CORE__OVERRIDE 
    "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
    "-DYQ_SHARE_DIR=\"${CMAKE_SOURCE_DIR}/share\"" 
)


IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqutil 
    PUBLIC 
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        ${GCC_FSLIB}
)


