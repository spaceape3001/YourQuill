////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Fragment>&, directory_t);
    bool            search(WebSearcher<Fragment>&, document_t);
    bool            search(WebSearcher<Fragment>&, folder_t);
    bool            search(WebSearcher<Fragment>&, parent_t);
    bool            search(WebSearcher<Fragment>&, root_t);
    FragmentVector  search(WebContext&, fragment_t);
}
