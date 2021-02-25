////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "util/Vector.hpp"
#include <QString>

struct Graph;

class GraphBuilder {
public:

    struct Node;
    struct Edge;

    GraphBuilder(const QString&);

    Node&               node(const QString&k, const QString& text=QString());
    Edge&               edge(const QString&, const QString&);

    void                rank(const QString&);
    const QString&      rank() const { return m_rank; }
    
    const QString&      bgcolor() const { return m_bgcolor; }
    void                bgcolor(const QString&); 
    
    Graph               build() const;
    
private:
    const QString       m_name;
    Vector<Node>        m_nodes;
    Vector<Edge>        m_edges;
    QString             m_bgcolor;
    QString             m_rank;
};


struct GraphBuilder::Node {
    const QString   key;
    QString         text;
    QString         url;
    QString         bgcolor;
    QString         fgcolor;
};

struct GraphBuilder::Edge {
    const QString   src, tgt;
    QString         color;
};

