////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CacheFwd.hpp"

struct Graph {
    uint64_t  id = 0ULL;
    constexpr auto    operator<=>(const Graph& rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};

namespace cdb {
    Vector<Graph>           all_graphs();
    size_t                  all_graphs_count();
    Graph                   db_graph(const String&dot_data, const String& name);
    String                  dot(Graph);
    bool                    exists(Graph);
    bool                    exists_graph(uint64_t);
    Graph                   graph(uint64_t);
    String                  html(Graph);
    String                  name(Graph);
    QByteArray              svg(Graph);
}
