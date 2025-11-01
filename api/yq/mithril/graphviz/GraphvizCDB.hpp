////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <vector>

namespace yq::mithril {
    struct Graphviz;
    namespace cdb {
        std::vector<Graphviz>   all_graphvizs(Sorted sorted=Sorted{});
        size_t                  all_graphvizs_count();
        Graphviz                db_graphviz(std::string_view dot_data, std::string_view name);
        ByteArray               dot_bytes(Graphviz);
        size_t                  dot_size(Graphviz);
        std::string             dot_text(Graphviz);
        bool                    exists(Graphviz);
        bool                    exists_graphviz(uint64_t);
        Graphviz                graphviz(uint64_t);
        ByteArray               html_bytes(Graphviz);
        size_t                  html_size(Graphviz);
        std::string             html_text(Graphviz);
        std::string             key(Graphviz);
        std::string             name(Graphviz);
        ByteArray               svg_bytes(Graphviz);
        size_t                  svg_size(Graphviz);
        std::string             svg_text(Graphviz);
    }
}
