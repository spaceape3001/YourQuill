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
        static thread_local CacheQuery s("SELECT id FROM Graphvizs");
        return s.vec<Graphviz>();
    }
    
    size_t                  all_graphvizs_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM Graphs");
        return s.size();
    }
    
    Graphviz                db_graphviz(std::string_view dot_data, std::string_view name)
    {
        std::string     html        = Graphviz::dot2html(dot_data);
        std::string     svg         = Graphviz::dot2svg(dot_data);
    
        static thread_local CacheQuery i("INSERT INTO Graphvizs (name, dot, html, svg) VALUES (?,?,?,?)");
        auto i_af = i.af();
        i.bind(1, name);
        i.bind(2, dot_data);
        i.bind(3, html);
        i.bind(4, svg);
        
        if(is_good(i.step(false))){
            return Graphviz((uint64_t) i.last_id());
        } else
            return Graphviz();
    }
    
    std::string             dot(Graphviz g)
    {
        static thread_local CacheQuery s("SELECT dot FROM Graphs WHERE id=?");
        return s.str(g);
    }
    
    bool                    exists(Graphviz g)
    {
        return exists_graphviz(g.id);
    }
    
    bool                    exists_graphviz(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM Graphs WHERE id=? LIMIT 1");
        return s.present(i);
    }
    
    Graphviz                graphviz(uint64_t i)
    {
        return exists_graphviz(i) ? Graphviz{i} : Graphviz{};
    }
    
    std::string             html_text(Graphviz g)
    {
        static thread_local CacheQuery s("SELECT html FROM Graphs WHERE id=?");
        return s.str(g);
    }
    
    std::string             key(Graphviz)
    {
        return std::string(); // TODO
    }
    
    std::string             name(Graphviz g)
    {
        static thread_local CacheQuery s("SELECT name FROM Graphs WHERE id=?");
        return s.str(g);
    }
    
    std::string             svg(Graphviz g)
    {
        static thread_local CacheQuery s("SELECT svg FROM Graphs WHERE id=?");
        return s.str(g);
    }
}
