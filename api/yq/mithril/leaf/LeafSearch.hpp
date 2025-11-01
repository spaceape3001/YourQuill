////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/keywords.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril {
    bool        search(WebSearcher<Leaf>&, folder_k);
    bool        search(WebSearcher<Leaf>&, tag_k);
    LeafVector  search(WebContext&, leaf_k);
}
