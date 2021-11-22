////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

GraphBuilder::GraphBuilder(const String&n) : m_name(n), m_rank("LR")
{
}

void              GraphBuilder::bgcolor(const String&s)
{
    m_bgcolor = s;
}

GraphBuilder::Edge& GraphBuilder::edge(const String&s, const String&t)
{
    m_edges << Edge{s,t};
    return m_edges.back();
}

GraphBuilder::Node& GraphBuilder::node(const String& k, const String& t)
{
    m_nodes << Node{k,t};
    return m_nodes.back();
}

void              GraphBuilder::rank(const String&s)
{
    m_rank = s;
}

Graph        GraphBuilder::build() const
{
    std::ostringstream  text;
    //String          bytes;
    //QTextStream     text(&bytes);
    //text.setCodec("UTF-8");
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

    return cdb::db_graph(text.view(), m_name);
}
