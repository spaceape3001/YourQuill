////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/Html.hpp>
#include <vector>

namespace yq {
    struct Attribute;
    
    namespace html {
        void    dev_table(WebHtml&, const std::vector<Attribute>&);
    }
}
