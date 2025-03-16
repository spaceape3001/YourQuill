////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/keywords.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    bool        search(WebSearcher<Leaf>&, folder_k);
    bool        search(WebSearcher<Leaf>&, tag_k);
    LeafVector  search(WebContext&, leaf_k);
}
