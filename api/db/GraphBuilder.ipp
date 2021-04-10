////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

GraphBuilder::GraphBuilder(const QString&n) : m_name(n), m_rank("LR")
{
}

void              GraphBuilder::bgcolor(const QString&s)
{
    m_bgcolor = s;
}

GraphBuilder::Edge& GraphBuilder::edge(const QString&s, const QString&t)
{
    m_edges << Edge{s,t};
    return m_edges.back();
}

GraphBuilder::Node& GraphBuilder::node(const QString& k, const QString& t)
{
    m_nodes << Node{k,t};
    return m_nodes.back();
}

void              GraphBuilder::rank(const QString&s)
{
    m_rank = s;
}

Graph        GraphBuilder::build() const
{
    QString         bytes;
    QTextStream     text(&bytes);
    //text.setCodec("UTF-8");
    text << "digraph " << m_name << "{\n";
    if(!m_rank.isEmpty())
        text << "    rankdir=" << m_rank << ";\n";
    if(!m_bgcolor.isEmpty())
        text << "    bgcolor=\"" << m_bgcolor << "\";\n";
    else
        text << "    bgcolor=\"#FFFFFF00\";\n";
    for(const Node& n : m_nodes){
        text << "    " << n.key << " [ shape=none, ";
        if(!n.url.isEmpty())
            text << "URL=\"" << n.url << "\", ";
        text << "label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPAdding=\"2\">"
             << "<TR><TD";
        if(!n.bgcolor.isEmpty())
            text << " BGCOLOR=\"" << n.bgcolor << "\"";
        text << ">";
        if(!n.fgcolor.isEmpty())
            text << "<FONT COLOR=\"" << n.fgcolor << "\">";
        text << n.text;
        if(!n.fgcolor.isEmpty())
            text << "</FONT>";
        text << "</TD></TR></TABLE>>];\n";
    }
    
    for(const Edge& e : m_edges){
        text << "    " << e.src << " -> " << e.tgt;
        if(!e.color.isEmpty())
            text << " [color=" << e.color << "]";
        text << ";\n";
    }
    
    text << "}\n";
    text.flush();

    return cdb::db_graph(bytes, m_name);
}
