////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Directory>&, folder_t);
    bool            search(WebSearcher<Directory>&, parent_t);
    bool            search(WebSearcher<Directory>&, root_t);
    DirectoryVector  search(WebContext&, directory_t);
}
