set(srv_sources
    #./ArgDecode.cpp
    #./ArgDecode.hpp
    ./CssPage.cpp
    ./CssPage.hpp
    ./CssWriter.hpp
    #./HtmlWriter.cpp
    #./HtmlWriter.hpp
    ./ImagePage.cpp
    ./ImagePage.hpp
    ./JsonPage.cpp
    ./JsonPage.hpp
    ./MarkdownPage.cpp
    ./MarkdownPage.hpp
    ./MarkdownWriter.hpp
    ./NetWriter.cpp
    ./NetWriter.hpp
    #./Page.cpp
    #./Page.hpp
    ./Session.cpp
    ./Session.hpp
    ./SrvLogging.hpp
    ./SvgPage.cpp
    ./SvgPage.hpp
    ./TextPage.cpp
    ./TextPage.hpp
    ./TextWriter.hpp
    ./TLSGlobals.cpp
    ./TLSGlobals.hpp
    ./TypedBytes.cpp
    ./TypedBytes.hpp
)

add_library(yqsrv SHARED ${srv_sources})
target_link_libraries(yqsrv PUBLIC yqdb QtWebApp Qt5::Svg)


