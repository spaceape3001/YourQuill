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
        struct Graph;
        namespace cdb {
            std::vector<Graph>      all_graphs(Sorted sorted=Sorted{});
            size_t                  all_graphs_count();
            Graph                   db_graph(std::string_view dot_data, std::string_view name);
            std::string             dot(Graph);
            bool                    exists(Graph);
            bool                    exists_graph(uint64_t);
            Graph                   graph(uint64_t);
            std::string             html_text(Graph);
            std::string             key(Graph);
            std::string             name(Graph);
            ByteArray               svg(Graph);
        }
    }
}
