////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace cdb {
     
    Vector<Graph>       all_graphs()
    {
        static thread_local SQ s("SELECT id FROM Graphs");
        return s.vec<Graph>();
    }
    
    size_t              all_graphs_count()
    {
        static thread_local SQ s("SELECT COUNT(1) FROM Graphs");
        return s.size();
    }

    Graph               db_graph(const String&dot_data, const String& name)
    {
        static const std::filesystem::path&   dot_exe = wksp::dot();
        
        QByteArray  html    = executeProcess(dot_exe.c_str(), QStringList() << "-Tcmapx", dot_data.c_str(), 500);
        QByteArray  svg     = executeProcess(dot_exe.c_str(), QStringList() << "-Tsvg", dot_data.c_str(), 500);
     
        static thread_local SQ i("INSERT INTO Graphs (name, dot, html, svg) VALUES (?,?,?,?)");
        auto i_af = i.af();
        i.bind(0, name);
        i.bind(1, dot_data);
        i.bind(2, utf8(html));
        i.bind(3, svg);
        if(!i.exec()){
            yError() << "Unable to create graph!";
            return Graph{};
        }
        
        return Graph(i.lastInsertIdU64());
    }

    String             dot(Graph g)
    {
        static thread_local SQ s("SELECT dot FROM Graphs WHERE id=?");
        return s.str(g.id);
    }


    bool                exists(Graph g)
    {
        return exists_graph(g.id);
    }

    bool                exists_graph(uint64_t i)
    {
        static thread_local SQ s("SELECT 1 FROM Graphs WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Graph               graph(uint64_t i)
    {
        return exists_graph(i) ? Graph{i} : Graph{};
    }

    String             html(Graph g)
    {
        static thread_local SQ s("SELECT html FROM Graphs WHERE id=?");
        return s.str(g.id);
    }

    String             name(Graph g)
    {
        static thread_local SQ s("SELECT name FROM Graphs WHERE id=?");
        return s.str(g.id);
    }

    QByteArray          svg(Graph g)
    {
        static thread_local SQ s("SELECT svg FROM Graphs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, g.id);
        if(s.exec() && s.next())
            return s.valueBytes(0);
        return QByteArray();
    }
}
