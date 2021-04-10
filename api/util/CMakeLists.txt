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
    ./Curl.cpp
    ./Curl.hpp
    ./DelayInit.cpp
    ./DelayInit.hpp
    ./Deque.hpp
    ./DiffEngine.hpp
    ./Enum.cpp
    ./Enum.hpp
    ./EnumMap.hpp
    ./Execute.cpp
    ./Execute.hpp
    ./FileName.hpp
    ./FileName.ipp
    ./FileUnity.cpp
    ./FileUtils.hpp
    ./FileUtils.ipp
    ./Flag.hpp
    ./Guarded.hpp
    ./Hash.hpp
    ./Http.cpp
    ./Http.hpp
    ./IntRange.hpp
    ./List.hpp
    ./LogFile.hpp
    ./LogFile.ipp
    ./Logging.cpp
    ./Logging.hpp
    ./Map.hpp
    ./MultiMap.hpp
    ./Random.cpp
    ./Random.hpp
    ./Ref.cpp
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
    ./String.cpp
    ./String.hpp
    ./TypeTraits.hpp
    ./Utilities.cpp
    ./Utilities.hpp
    ./Vector.hpp
    ./XmlFile.hpp
    ./XmlFile.ipp
    ./XmlFwd.hpp
    ./XmlUtils.hpp
    ./XmlUtils.ipp
    ./ZipFile.cpp
    ./ZipReader.hpp
    ./ZipWriter.hpp

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


