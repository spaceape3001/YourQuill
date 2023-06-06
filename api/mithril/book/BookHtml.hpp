////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>

namespace yq::mithril {
    struct Book;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Book);
        WebHtml&    operator<<(WebHtml&, Dev<Book>);
        WebHtml&    operator<<(WebHtml&, DevID<Book>);
        void        dev_table(WebHtml&, const std::vector<Book>&);
        
    }
}
