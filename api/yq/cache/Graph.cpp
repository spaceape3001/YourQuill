////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Graph.hpp"
#include "yq/Loggers.hpp"
#include "yq/io/Execute.hpp"
#include "yq/sql/SqlQuery.hpp"
#include "yq/util/Utilities.hpp"
#include "yq/util/Vector.hpp"
#include "yq/wksp/Workspace.hpp"

namespace cdb {
     
    Vector<Graph>       all_graphs()
    {
        Vector<Graph>   ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Graphs");
        auto s_af = s.af();
        if(s.exec()){
            while(s.next())
                ret << Graph{ s.valueU64(0) };
        }
        return ret;
    }
    
    size_t              all_graphs_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Graphs");
        auto s_af   = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }

    Graph               db_graph(const QString&dot_data, const QString& name)
    {
        static const QString&   dot_exe = wksp::dot();
        
        QByteArray  html    = executeProcess(dot_exe, QStringList() << "-Tcmapx", dot_data.toUtf8(), 500);
        QByteArray  svg     = executeProcess(dot_exe, QStringList() << "-Tsvg", dot_data.toUtf8(), 500);
     
        static thread_local SqlQuery i(wksp::cache(), "INSERT INTO Graphs (name, dot, html, svg) VALUES (?,?,?,?)");
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

    QString             dot(Graph g)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT dot FROM Graphs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, g.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }


    bool                exists(Graph g)
    {
        return exists_graph(g.id);
    }

    bool                exists_graph(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Graphs WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    Graph               graph(uint64_t i)
    {
        return exists_graph(i) ? Graph{i} : Graph{};
    }

    QString             html(Graph g)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT html FROM Graphs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, g.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    QString             name(Graph g)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT name FROM Graphs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, g.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    QByteArray          svg(Graph g)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT svg FROM Graphs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, g.id);
        if(s.exec() && s.next())
            return s.valueBytes(0);
        return QByteArray();
    }
}
