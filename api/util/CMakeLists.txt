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
    
    ./AbstractFile.ipp
    ./AbstractFile.hpp
    ./Array1.hpp
    ./Array2.hpp
    ./ByteArray.hpp
    ./ByteArray.ipp
    ./Char8.hpp
    ./Char8.ipp
    ./Char32.hpp
    ./Char32.ipp
    ./CmdArgs.cpp
    ./CmdArgs.hpp
    ./Comma.hpp
    ./Compare.hpp
    ./CoreUnity.cpp
    ./Curl.ipp
    ./Curl.hpp
    ./DelayInit.ipp
    ./DelayInit.hpp
    ./Deque.hpp
    ./DiffEngine.hpp
    ./Enum.ipp
    ./Enum.hpp
    ./EnumMap.hpp
    ./Execute.ipp
    ./Execute.hpp
    ./FileName.hpp
    ./FileName.ipp
    ./FileUtils.hpp
    ./FileUtils.ipp
    ./Flag.hpp
    ./Format.hpp
    ./Guarded.hpp
    ./Hash.hpp
    ./Http.ipp
    ./Http.hpp
    ./IgCase.hpp
    ./IgCase.ipp
    ./IntRange.hpp
    ./IOUnity.cpp
    ./List.hpp
    ./LogFile.hpp
    ./LogFile.ipp
    ./Logging.hpp
    ./Logging.ipp
    ./Map.hpp
    ./MultiMap.hpp
    ./Random.ipp
    ./Random.hpp
    ./Ref.ipp
    ./Ref.hpp
    ./Safety.hpp
    ./Set.hpp
    ./Span.hpp
    ./SqlQuery.ipp
    ./SqlQuery.hpp
    ./SqlUnity.cpp
    ./SqlUtils.ipp
    ./SqlUtils.hpp
    ./Stack.hpp
    ./Stream.hpp
    # ./StreamOps.ipp
    ./String.ipp
    ./String.hpp
    ./StringUtil.ipp
    ./StringUtil.hpp
    ./StringView.ipp
    ./StringView.hpp
    ./TypeTraits.hpp
    ./Url.ipp
    ./Url.hpp
    ./Utilities.ipp
    ./Utilities.hpp
    ./Vector.hpp
    ./XmlFile.hpp
    ./XmlFile.ipp
    ./XmlFwd.hpp
    ./XmlUtils.hpp
    ./XmlUtils.ipp
    
    #   Disabled until needed
    #./ZipFile.cpp
    #./ZipReader.hpp
    #./ZipWriter.hpp

    ./stream/Bytes.hpp
    ./stream/File.hpp
    ./stream/OSFile.hpp
    ./stream/LineCol.hpp
    ./stream/Ops.hpp
    ./stream/Streams.cpp
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


