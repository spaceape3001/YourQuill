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
    
    #./AbstractFile.ipp
    #./AbstractFile.hpp
    #./Compare.hpp
    #./CoreUnity.cpp
    #./Curl.ipp
    #./Curl.hpp
    #./DiffEngine.hpp
    #./Enum.ipp
    #./Enum.hpp
    #./Execute.ipp
    #./Execute.hpp
    #./FileName.hpp
    #./FileName.ipp
    #./FileUtils.hpp
    #./FileUtils.ipp
    #./Flag.hpp
    #./Format.hpp
    #./Guarded.hpp
    #./Http.ipp
    #./Http.hpp
    #./IntRange.hpp
    #./IOUnity.cpp
    #./LogFile.hpp
    #./LogFile.ipp
    #./Logging.hpp
    #./Logging.ipp
    #./Random.ipp
    #./Random.hpp
    #./Ref.ipp
    #./Ref.hpp
    #./SqlQuery.ipp
    #./SqlQuery.hpp
    #./SqlUnity.cpp
    #./SqlUtils.ipp
    #./SqlUtils.hpp
    #./Stream.hpp
    ## ./StreamOps.ipp
    #./String.ipp
    #./String.hpp
    #./StringUtil.ipp
    #./StringUtil.hpp
    #./StringView.ipp
    #./StringView.hpp
    #./Url.ipp
    #./Url.hpp
    #./XmlFile.hpp
    #./XmlFile.ipp
    #./XmlFwd.hpp
    #./XmlUtils.hpp
    #./XmlUtils.ipp
    
    #   Disabled until needed
    #./ZipFile.cpp
    #./ZipReader.hpp
    #./ZipWriter.hpp
    
    ./Utilities.cpp
    ./Utilities.hpp

    ./algo/AlgoUnity.cpp
    ./algo/Compare.hpp
    ./algo/DiffEngine.hpp
    ./algo/Random.hpp
    
    ./app/AppUnity.cpp
    ./app/CmdArgs.hpp
    ./app/DelayInit.hpp

    ./c++/C++.cpp
    ./c++/Concat.hpp
    ./c++/DebugRel.hpp
    ./c++/NaN.hpp
    ./c++/NoCopy.hpp
    ./c++/NoMove.hpp
    ./c++/Safety.hpp
    ./c++/TypeTraits.hpp
    ./c++/UInt128.hpp

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
    ./file/FileUnity.cpp
    ./file/FileUtils.hpp
    ./file/XmlFile.hpp
    ./file/XmlFwd.hpp
    ./file/XmlUtils.hpp
    
    ./log/LogFile.cpp
    ./log/LogFile.hpp
    ./log/Logging.cpp
    ./log/Logging.hpp
    
    ./net/Curl.cpp
    ./net/Curl.hpp
    ./net/Http.cpp
    ./net/Http.hpp
    ./net/Url.cpp
    ./net/Url.hpp

    ./proc/Execute.cpp
    ./proc/Execute.hpp
    
    ./sql/SqlLogging.hpp
    ./sql/SqlQuery.hpp
    ./sql/SqlUnity.cpp
    ./sql/SqlUtils.hpp

    ./stream/Bytes.hpp
    ./stream/File.hpp
    ./stream/OSFile.hpp
    ./stream/LineCol.hpp
    ./stream/Ops.hpp
    ./stream/Streams.cpp
    
    ./text/ByteArray.hpp
    ./text/Char8.hpp
    ./text/Char32.hpp
    ./text/Comma.hpp
    ./text/Format.hpp
    ./text/IgCase.hpp
    ./text/String.hpp
    ./text/StringPair.hpp
    ./text/StringUtil.hpp
    ./text/StringView.hpp
    ./text/Strings.hpp
    ./text/TextUnity.cpp
    
    ./type/Enum.hpp
    ./type/Flag.hpp
    ./type/IntRange.hpp
    ./type/Ref.hpp
    ./type/Result.hpp
    ./type/Span.hpp
    ./type/Touched.hpp
    ./type/TypeUnity.cpp
)

add_library(yqutil SHARED ${util_sources})
target_compile_definitions(yqutil PRIVATE YQ__CORE__OVERRIDE )

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqutil 
    PUBLIC 
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        Qt5::Core 
        Qt5::Gui 
        Qt5::Sql
        ${GCC_FSLIB}
)


