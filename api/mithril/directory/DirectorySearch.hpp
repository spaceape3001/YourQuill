////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/keywords.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Directory>&, folder_k);
    bool            search(WebSearcher<Directory>&, parent_k);
    bool            search(WebSearcher<Directory>&, root_k);
    DirectoryVector  search(WebContext&, directory_k);
}
