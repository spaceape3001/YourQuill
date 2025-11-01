////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/keywords.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Attribute>&, document_k);

    AttributeVector  search(WebContext&, attribute_k);
}
