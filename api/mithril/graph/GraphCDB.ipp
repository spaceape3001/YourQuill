////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Graph.hpp"
#include "GraphCDB.hpp"

namespace yq::mithril::cdb {
    std::vector<Graph>      all_graphs(Sorted sorted)
    {
        return std::vector<Graph>();    // TODO
    }
    
    size_t                  all_graphs_count()
    {
        return 0ULL;    // TODO
    }
    
    Graph                   db_graph(std::string_view dot_data, std::string_view name)
    {
        return Graph(); // TODO
    }
    
    std::string             dot(Graph)
    {
        return std::string();   // TODO
    }
    
    bool                    exists(Graph)
    {
        return false;   // TODO
    }
    
    bool                    exists_graph(uint64_t)
    {
        return false; // TODO
    }
    
    Graph                   graph(uint64_t)
    {
        return Graph(); // TODO
    }
    
    std::string             html_text(Graph)
    {
        return std::string(); // TODO
    }
    
    std::string             key(Graph)
    {
        return std::string(); // TODO
    }
    
    std::string             name(Graph)
    {
        return std::string(); // TODO
    }
    
    ByteArray               svg(Graph)
    {
        return ByteArray(); // TODO
    }
}
