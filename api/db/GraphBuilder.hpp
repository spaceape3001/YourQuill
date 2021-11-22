////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/String.hpp>
#include <util/Vector.hpp>

struct Graph;

class GraphBuilder {
public:

    struct Node;
    struct Edge;

    GraphBuilder(const String&);

    Node&               node(const String&k, const String& text=String());
    Edge&               edge(const String&, const String&);

    void                rank(const String&);
    const String&       rank() const { return m_rank; }
    
    const String&       bgcolor() const { return m_bgcolor; }
    void                bgcolor(const String&); 
    
    Graph               build() const;
    
private:
    const String        m_name;
    Vector<Node>        m_nodes;
    Vector<Edge>        m_edges;
    String              m_bgcolor;
    String              m_rank;
};


struct GraphBuilder::Node {
    const String    key;
    String          text;
    String          url;
    String          bgcolor;
    String          fgcolor;
};

struct GraphBuilder::Edge {
    const String    src, tgt;
    String          color;
};

