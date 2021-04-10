set(srv_sources
    ./ArgDecode.hpp
    ./ArgDecode.ipp
    ./CssPage.hpp
    ./CssPage.ipp
    ./CssWriter.hpp
    ./HtmlPage.hpp
    ./HtmlPage.ipp
    ./HtmlWriter.hpp
    ./ImagePage.hpp
    ./ImagePage.ipp
    ./Importer.hpp
    ./Importer.ipp
    ./JsonPage.hpp
    ./JsonPage.ipp
    ./MarkdownPage.hpp
    ./MarkdownPage.ipp
    ./MarkdownWriter.hpp
    ./NetPageUnity.cpp
    ./NetWriter.hpp
    ./NetWriter.ipp
    ./Page.hpp
    ./Page.ipp
    ./Scanner.hpp
    ./Scanner.ipp
    ./Session.hpp
    ./Session.ipp
    ./SrvLogging.hpp
    ./SrvUnity.cpp
    ./StdPage.ipp
    ./SvgPage.hpp
    ./SvgPage.ipp
    ./TextPage.hpp
    ./TextPage.ipp
    ./TextWriter.hpp
    ./TLSGlobals.hpp
    ./TLSGlobals.ipp
    ./TypedBytes.hpp
    ./TypedBytes.ipp
    ./Utilities.hpp
    ./Utilities.ipp
)

add_library(yqsrv SHARED ${srv_sources})
target_link_libraries(yqsrv PUBLIC yqdb QtWebApp Qt5::Svg)


