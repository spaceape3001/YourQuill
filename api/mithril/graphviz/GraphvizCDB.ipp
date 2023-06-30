////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Graphviz.hpp"
#include "GraphvizCDB.hpp"

namespace yq::mithril::cdb {
    std::vector<Graphviz>      all_graphvizs(Sorted sorted)
    {
        return std::vector<Graphviz>();    // TODO
    }
    
    size_t                  all_graphvizs_count()
    {
        return 0ULL;    // TODO
    }
    
    Graphviz                db_graphviz(std::string_view dot_data, std::string_view name)
    {
        return Graphviz(); // TODO
    }
    
    std::string             dot(Graphviz)
    {
        return std::string();   // TODO
    }
    
    bool                    exists(Graphviz)
    {
        return false;   // TODO
    }
    
    bool                    exists_graphviz(uint64_t)
    {
        return false; // TODO
    }
    
    Graphviz                graphviz(uint64_t)
    {
        return Graphviz(); // TODO
    }
    
    std::string             html_text(Graphviz)
    {
        return std::string(); // TODO
    }
    
    std::string             key(Graphviz)
    {
        return std::string(); // TODO
    }
    
    std::string             name(Graphviz)
    {
        return std::string(); // TODO
    }
    
    ByteArray               svg(Graphviz)
    {
        return ByteArray(); // TODO
    }
}
