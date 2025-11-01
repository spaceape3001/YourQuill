////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Document);
    WebHtml&    operator<<(WebHtml&, Dev<Document>);
    WebHtml&    operator<<(WebHtml&, DevID<Document>);
    WebHtml&    operator<<(WebHtml&, const Icon<Document>&);

    void    dev_table(WebHtml&, const DocumentVector& documents);
    
    //! Queries for a map of extensions to icons (in www/img/ext)
    //! \note This requeries EVERY time
    string_map_t    make_file_extension_icons();
    void    dev_title(WebHtml&, Document, std::string_view extra=std::string_view());

    //! Returns a map of extensions to icons (in www/img/ext)
    //! \note New extensions require a restart as this is done *ONCE* and recycled thereafter
    const string_map_t& file_extension_icons();
}

