set(srv_sources
    ./ArgDecode.cpp
    ./ArgDecode.hpp
    ./CssPage.cpp
    ./CssPage.hpp
    ./CssWriter.hpp
    ./HtmlPage.cpp
    ./HtmlPage.hpp
    ./HtmlWriter.hpp
    ./ImagePage.cpp
    ./ImagePage.hpp
    ./Importer.cpp
    ./Importer.hpp
    ./JsonPage.cpp
    ./JsonPage.hpp
    ./MarkdownPage.cpp
    ./MarkdownPage.hpp
    ./MarkdownWriter.hpp
    ./NetWriter.cpp
    ./NetWriter.hpp
    ./Page.cpp
    ./Page.hpp
    ./Session.cpp
    ./Session.hpp
    ./SrvLogging.hpp
    ./StdPage.cpp
    ./SvgPage.cpp
    ./SvgPage.hpp
    ./TextPage.cpp
    ./TextPage.hpp
    ./TextWriter.hpp
    ./TLSGlobals.cpp
    ./TLSGlobals.hpp
    ./TypedBytes.cpp
    ./TypedBytes.hpp
    ./Utilities.cpp
    ./Utilities.hpp
)

add_library(yqsrv SHARED ${srv_sources})
target_link_libraries(yqsrv PUBLIC yqdb QtWebApp Qt5::Svg)


