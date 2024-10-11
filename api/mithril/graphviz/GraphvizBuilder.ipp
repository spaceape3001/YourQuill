////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/graphviz/GraphvizBuilder.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>
#include <yq/core/StreamOps.hpp>
#include <yq/stream/Text.hpp>

namespace yq::mithril {
    Graphviz::Node&             Graphviz::Builder::node(std::string_view k, std::string_view txt)
    {
        m_nodes.push_back(Node(k,txt));
        return m_nodes.back();
    }
    
    Graphviz::Edge&             Graphviz::Builder::edge(std::string_view s, std::string_view t)
    {
        m_edges.push_back(Edge(s,t));
        return m_edges.back();
    }
    
    std::string                 Graphviz::Builder::make_dot() const
    {
        std::string     bytes;
        stream::Text    text(bytes);
        text << "digraph " << m_name << "{\n";
        if(!m_rank.empty())
            text << "    rankdir=" << m_rank << ";\n";
        if(!m_bgcolor.empty())
            text << "    bgcolor=\"" << m_bgcolor << "\";\n";
        else
            text << "    bgcolor=\"#FFFFFF00\";\n";
        for(const Node& n : m_nodes){
            text << "    " << n.key << " [ shape=none, ";
            if(!n.url.empty())
                text << "URL=\"" << n.url << "\", ";
            text << "label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPAdding=\"2\">"
                 << "<TR><TD";
            if(!n.bgcolor.empty())
                text << " BGCOLOR=\"" << n.bgcolor << "\"";
            text << ">";
            if(!n.fgcolor.empty())
                text << "<FONT COLOR=\"" << n.fgcolor << "\">";
            text << n.text;
            if(!n.fgcolor.empty())
                text << "</FONT>";
            text << "</TD></TR></TABLE>>];\n";
        }
        
        for(const Edge& e : m_edges){
            text << "    " << e.src << " -> " << e.tgt;
            if(!e.color.empty())
                text << " [color=" << e.color << "]";
            text << ";\n";
        }
        
        text << "}\n";
        text.flush();
        return bytes;
    }
    
    Graphviz                    Graphviz::Builder::build() const
    {
        return cdb::db_graphviz(make_dot(), m_name);
    }
}
