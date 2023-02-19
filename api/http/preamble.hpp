////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/keywords.hpp>

namespace yq {
    struct WebContext;
    struct Root;
    
    class ByteArray;
    class HtmlTable;
    class Stream;
    class WebAutoClose;
    class WebHtml;

    namespace html {
        template <typename T> struct Config;
        template <typename T> struct Dev;
        template <typename T> struct DevID;
        template <typename T> struct Edit;
        template <typename T> struct Full;
        template <typename T> struct Icon;
        template <typename T> struct LinkKey;
        template <typename T> struct LinkLabel;
        template <typename T> struct LinkThumb;
        template <typename T> struct Plural;
        template <typename T> struct Thumb;

        struct Escape;
        struct ICheck;
        struct IEditReq;
        struct IKey;
        struct IHidden;
        struct ILabel;
        struct ILine;
        struct ISubmit;
        struct IText;
        struct MarkdownText;
    }
}
