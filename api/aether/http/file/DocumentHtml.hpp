////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/http/Html.hpp>

namespace yq {
    struct Document;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Document);
        WebHtml&    operator<<(WebHtml&, Dev<Document>);
        WebHtml&    operator<<(WebHtml&, DevID<Document>);
        WebHtml&    operator<<(WebHtml&, const Icon<Document>&);
        void        dev_table(WebHtml&, const std::vector<Document>&);

        //! Queries for a map of extensions to icons (in www/img/ext0
        //! \note This requeries EVERY time
        string_map_t    make_file_extension_icons();

        //! Returns a map of extensions to icons (in www/img/ext)
        //! \note New extensions require a restart as this is done *ONCE* and recycled thereafter
        const string_map_t& file_extension_icons();
    }
}


