////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/keywords.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Fragment>&, directory_k);
    bool            search(WebSearcher<Fragment>&, document_k);
    bool            search(WebSearcher<Fragment>&, folder_k);
    bool            search(WebSearcher<Fragment>&, parent_k);
    bool            search(WebSearcher<Fragment>&, root_k);
    FragmentVector  search(WebContext&, fragment_k);
}
