add_library(RapidXml INTERFACE)
target_sources(RapidXml INTERFACE
    ./rapidxml.hpp 
    ./rapidxml_iterators.hpp 
    ./rapidxml_print.hpp 
    ./rapidxml_utils.hpp
)
target_include_directories(RapidXml INTERFACE .)

