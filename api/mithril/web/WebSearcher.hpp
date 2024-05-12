////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>

namespace yq::mithril {
    template <typename T>
    struct WebSearcher {
        WebContext&         ctx;
        std::vector<T>      data;
        bool                first  = true;
        WebSearcher(WebContext& c) : ctx(c) {}
    };
}
