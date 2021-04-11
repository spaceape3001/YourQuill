#   Some of the major libraries will be combined into a single "unity-style" library for
#   cohesiveness (and the ability to cross-link).  Other libraries will be added independently.
#
#   Files should be listed alphabetically

set(util_sources
    ./AbstractFile.ipp
    ./AbstractFile.hpp
    ./Array1.hpp
    ./Array2.hpp
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
    ./Guarded.hpp
    ./Hash.hpp
    ./Http.ipp
    ./Http.hpp
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
    ./String.ipp
    ./String.hpp
    ./TypeTraits.hpp
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

)

add_library(yqutil SHARED ${util_sources})
target_compile_definitions(yqutil PRIVATE YQ__CORE__OVERRIDE )

target_link_libraries(yqutil 
    PUBLIC 
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        Qt5::Core 
        Qt5::Gui 
        Qt5::Sql
)


