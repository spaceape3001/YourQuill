////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/CacheFwd.hpp>
#include <vector>

namespace yq {
    class ByteArray;
    namespace cdb {
        struct Graph;

        std::vector<Graph>      all_graphs();
        size_t                  all_graphs_count();
        Graph                   db_graph(std::string_view dot_data, std::string_view name);
        std::string             dot(Graph);
        bool                    exists(Graph);
        bool                    exists_graph(uint64_t);
        Graph                   graph(uint64_t);
        std::string             html_text(Graph);
        std::string             name(Graph);
        ByteArray               svg(Graph);
    }
}
