////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/graphviz/Graphviz.hpp>

namespace yq::mithril {

    //! Simple Node for graphviz
    struct Graphviz::Node {
        const std::string       key;
        std::string             text;
        std::string             url;
        std::string             bgcolor;
        std::string             fgcolor;
        
        Node(){}
        Node(std::string_view k, std::string_view txt) : key(k), text(txt) {}
        
        constexpr bool operator==(const Node&) const noexcept = default;
    };

    //! Simple Edge for graphviz
    struct Graphviz::Edge {
        const std::string       src, tgt;
        std::string             color;

        Edge(){}
        Edge(std::string_view s, std::string_view t) : src(s), tgt(t) {}

        constexpr bool operator==(const Edge&) const noexcept = default;
    };

    /*! \brief Utility for building graphs
    */
    struct Graphviz::Builder {
    
        Builder(std::string_view n) : m_name(n) {}
        ~Builder() = default;
        
        //! Creates a node
        Node&                       node(std::string_view, std::string_view txt=std::string_view());
        
        //! Creates an edge (does not verify nodes exist);
        Edge&                       edge(std::string_view, std::string_view);;
        
        std::string                 make_dot() const;
        Graphviz                    build() const;

        const std::string           m_name;
        std::vector<Node>           m_nodes;
        std::vector<Edge>           m_edges;
        std::string                 m_bgcolor;
        std::string                 m_rank;
    };

}

