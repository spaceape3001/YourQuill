////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <vector>

namespace yq {
    namespace mithril {
        struct Graphviz;
        namespace cdb {
            std::vector<Graphviz>   all_graphvizs(Sorted sorted=Sorted{});
            size_t                  all_graphvizs_count();
            Graphviz                db_graphviz(std::string_view dot_data, std::string_view name);
            std::string             dot(Graphviz);
            bool                    exists(Graphviz);
            bool                    exists_graphviz(uint64_t);
            Graphviz                graphviz(uint64_t);
            std::string             html_text(Graphviz);
            std::string             key(Graphviz);
            std::string             name(Graphviz);
            std::string             svg(Graphviz);
        }
    }
}
