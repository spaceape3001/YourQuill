////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct Graphviz;

    namespace arg {
        //! Finds graphviz by ID
        Graphviz graphviz(std::string_view arg_string);
        Graphviz graphviz(const WebContext&, bool *detected=nullptr);
        Graphviz graphviz(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Graphviz graphviz(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
