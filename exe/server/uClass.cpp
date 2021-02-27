////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yUpdater.hpp"
#include "yImporter.hpp"
#include "yCommon.hpp"

#include "db/Cache.hpp"
#include "db/GraphBuilder.hpp"
#include "util/Logging.hpp"
#include "util/SqlQuery.hpp"
#include "util/Guarded.hpp"
#include "util/Utilities.hpp"

#include <QTextStream>
#include <QThreadPool>

namespace {
    using CPair = std::pair<Class,Class>;
    using FPair = std::pair<Class,Field>;

    Guarded<Graph>          gClassDep;

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  READING of the classes from the drive

    
        //! Updates class data that does NOT refer to another class
    void    on_interior_read(uClass& c)
    {
        ClassFile ::Shared    whole = cdb::merged(c.cls);
        
        if(cdb::fragments_count(c.doc) > 0){
            static thread_local SqlQuery    u(wksp::cache(), "UPDATE Classes SET name=?,plural=?,brief=?,removed=0 WHERE id=?");
            auto u_af   = u.af();
            u.bind(0, whole->name);
            u.bind(1, whole->plural);
            u.bind(2, whole->brief);
            u.bind(3, c.cls.id);
            u.exec();
        }

        c.fields.clear();
        for(auto& j : whole->fields){
            Field   f   = cdb::db_field(c.cls, j.first.qString());
            c.fields << f;
        }
    }


        //! Updates class data that does refer to another class
    void    on_exterior_read(uClass& c)
    {
        //  This routine updates whatever references another class
        ClassFile::Shared        whole = cdb::merged(c.cls);
        c.use.clear();
        c.src.clear();
        c.tgt.clear();
        
        for(auto j : whole->use)
            c.use << cdb::db_class(j.qString());
        for(auto j : whole->sources)
            c.src << cdb::db_class(j.qString());
        for(auto j : whole->targets)
            c.tgt << cdb::db_class(j.qString());
        for(auto j : whole->reverse)
            c.rev << cdb::db_class(j.qString());
    }
    
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  DATABASES

    //void    db_leaf_table(uClass& c)
    //{
        //QString     stmt;
        //{
            //QTextStream txt(&stmt);
            //txt << "CREATE TABLE " << c.db << " ( id PRIMARY KEY";
            
            
            //txt << ")";
            //txt.flush();
        //}
        //SqlQuery    ct(wksp::cache(), stmt);
        //ct.exec();
    //}

    void    create_db(uClass& c)
    {
        for(Field ff : c.fields){
            uField& f   = uget(ff);
            
            f.db        = QString("%1_%2").arg(cdb::key(c.cls)).arg(cdb::key(ff));
            
            SqlQuery(wksp::cache(), QString(
                "CREATE TABLE %1 ( "
                    "leaf INTEGER, "
                    "attr VARCHAR(255) COLLATE NOCASE, "
                    "value VARCHAR(255), "
                    "UNIQUE(leaf,attr)"
                ")").arg(f.db)).exec();
        
        }
    
        //db_leaf_table(c);
    }


        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  GRAPHS
    

    Graph   draw_class_graph(const Vector<Class>& classes, const Vector<cdb::ClassPair>& edges, Class focus, const QString& url /* assumes class id to be appended&cls=? will */ )
    {
        GraphBuilder    gb("classGraph");
        bool            doUrl   = !url.isEmpty();
        QString         tcolor  = cur_text_color();
        QString         bcolor  = cur_back_color();
        
        gb.rank("RL");
        
        for(Class c : classes){
            QString k   = cdb::key(c);
            QString t   = cdb::name(c);
            if(t.isEmpty())
                t       = k;
            auto & n = gb.node("cl_" + k, t);
            if(doUrl && (c != focus))
                n.url   = url + k;
            if(c != focus){
                n.bgcolor   = tcolor;
                n.fgcolor   = bcolor;
            } else {
                n.bgcolor   = bcolor;
                n.fgcolor   = tcolor;
            }
        }
        
        for(auto j : edges){
            auto& e = gb.edge("cl_" + cdb::key(j.first), "cl_" + cdb::key(j.second));
            e.color = tcolor;
        }
        
        return gb.build();
    }


    
        //  Class depenedency generator
    struct ClassDependencies {
        Map<Class,Set<Class>>       uses, deriveds;
        
        ClassDependencies(const Vector<cdb::ClassPair>& allDeps)
        {
            for(auto i : allDeps){
                uses[i.first] << i.second;
                deriveds[i.second] << i.first;
            }
        }
        
        void        eval(Class c, Set<Class>& classes, Set<cdb::ClassPair>& edges, unsigned int up, unsigned int down) 
        {
            if(up){
                for(auto u : uses[c]){
                    bool    f   = classes.add(u);
                    bool    g   = edges.add({c,u});
                    if(g&&f)
                        eval(u,classes,edges,up-1, (down>1)?(down-2):0 );
                }
            }
            
            if(down){
                for(auto d : deriveds[c]){
                    bool    f   = classes.add(d);
                    bool    g   = edges.add({d,c});
                    if(g&&f)
                        eval(d,classes,edges,up?(up-1):0,down-1);
                }
            }
        }
        
        void        get(Class c, Vector<Class>& classes, Vector<cdb::ClassPair>& edges, unsigned int up, unsigned int down) 
        {
            Set<Class>          retClass;
            Set<cdb::ClassPair>   retEdge;
            retClass << c;
            eval(c, retClass, retEdge, up, down);
            classes     = makeVector(retClass);
            edges       = makeVector(retEdge);
        }
        
    };


    void    gen_class_dep()
    {
        static thread_local SqlQuery u(wksp::cache(), "UPDATE Classes SET deps=? WHERE id=?");
        Vector<Class>           allC    = cdb::all_classes();
        Vector<cdb::ClassPair>  allU    = cdb::all_class_dependencies();
        ClassDependencies       cdeps(allU);
        
        gClassDep   = draw_class_graph(allC, allU, Class{}, "/wksp/class?cls=");
        for(Class c : allC){
            Vector<Class>               subC;
            Vector<cdb::ClassPair>    subU;
            cdeps.get(c, subC, subU, 1, 3);
            
            Graph   g   = draw_class_graph(subC, subU, c, "/wksp/class/dependency?cls=");
            if(g){
                auto u_af   = u.af();
                u.bind(0, g.id);
                u.bind(1, c.id);
                u.exec();
            }
        }
    }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  DEPENDENCIES
    
    
    void    analyze_dependencies(Map<cdb::ClassPair,int>& deps, Class c1, const Set<Class>& use, int depth)
    {
        for(Class c2 : use){
            if(c1 == c2)                // catch circular dependencies
                continue;
            if(deps.has(CPair(c1,c2)))
                continue;
            deps[CPair(c1,c2)]  = depth;
            analyze_dependencies(deps, c1, uget(c2).use, depth+1);
        }
    }


    void    analyze_classes(bool doGraphs)
    {
        static Map<cdb::ClassPair,int>       s_depends;
        static Map<cdb::ClassPair,int>       s_sources;
        static Map<cdb::ClassPair,int>       s_targets;
        static Map<cdb::ClassPair,int>       s_reverse;
        static Map<FPair,int>       s_fields;
    
        std::vector<Class>  all             = cdb::all_classes();
        Map<cdb::ClassPair,int>      depends, sources, targets, reverse;
        Map<FPair,int>      fields;
        
        
        //  First, handle the "dependencies", sources, targets, fields
        for(Class c : all)
            analyze_dependencies(depends, c, uget(c).use, 0);

        //  Seed the others
        for(Class c : all){
            uClass& u   = uget(c);
            for(Class c1 : u.src)
                sources[CPair(c,c1)]    = 0;
            for(Class c1 : u.tgt)
                targets[CPair(c,c1)]    = 0;
            for(Class c1 : u.rev)
                reverse[CPair(c,c1)]    = 0;
            for(Field f : u.fields)
                fields[FPair(c,f)]      = 0;
        }
        

        //  Now, merege the stuff together....
        for(auto i : depends){
            Class c0=i.first.first;
            uClass& u = uget(i.first.second);
            int k = i.second + 1;
            for(Class c1 : u.src){
                auto j = sources.find(CPair(c0,c1));
                if(j != sources.end()){
                    j->second   = std::min(j->second, k);
                } else
                    sources[CPair(c0,c1)]  = k;
            }
            for(Class c1 : u.tgt){
                auto j = targets.find(CPair(c0,c1));
                if(j != targets.end()){
                    j->second   = std::min(j->second, k);
                } else
                    targets[CPair(c0,c1)]  = k;
            }
            for(Class c1 : u.rev){
                auto j = reverse.find(CPair(c0, c1));
                if(j != reverse.end()){
                    j->second   = std::min(j->second,k);
                } else
                    reverse[CPair(c0,c1)]  = k;
            }
            for(Field f1 : u.fields){
                auto j = fields.find(FPair(c0, f1));
                if(j != fields.end()){
                    j->second   = std::min(j->second, k);
                } else
                    fields[FPair(c0,f1)]   = k;
            }
        }

        
        MapReport<cdb::ClassPair>        d_depends, d_sources, d_targets, d_reverse;
        MapReport<FPair>        d_fields;
        
        map_report(s_depends, depends, d_depends);
        map_report(s_sources, sources, d_sources);
        map_report(s_targets, targets, d_targets);
        map_report(s_reverse, reverse, d_reverse);
        map_report(s_fields,  fields, d_fields);
        
        if(!d_depends.removed.empty()){
            QVariantList    id, base;
            
            for(auto i : d_depends.removed){
                id << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CDepends WHERE id=? AND base=?");
            auto d_af = d.af();
            d.addBindValue(id);
            d.addBindValue(base);
            d.batch();
        }
        if(!d_depends.changed.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_depends.changed){
                int j   = depends[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery u(wksp::cache(), "UPDATE CDepends SET hops=? WHERE id=? AND base=?");
            auto u_af = u.af();
            
            u.addBindValue(hops);
            u.addBindValue(id);
            u.addBindValue(base);
            u.batch();
        }
        if(!d_depends.added.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_depends.added){
                int j   = depends[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }

            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CDepends (id,base,hops) VALUES (?,?,?)");
            auto i_af   = i.af();
            i.addBindValue(id);
            i.addBindValue(base);
            i.addBindValue(hops);
            i.batch();
        }
        if(!d_sources.removed.empty()){
            QVariantList    id, base;
            
            for(auto i : d_sources.removed){
                id << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CSources WHERE id=? AND src=?");
            auto d_af = d.af();
            d.addBindValue(id);
            d.addBindValue(base);
        }
        if(!d_sources.changed.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_sources.changed){
                int j   = sources[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery u(wksp::cache(), "UPDATE CSources SET hops=? WHERE id=? AND src=?");
            auto u_af = u.af();
            
            u.addBindValue(hops);
            u.addBindValue(id);
            u.addBindValue(base);
            u.batch();
        }
        if(!d_sources.added.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_sources.added){
                int j   = sources[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }

            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CSources (id,src,hops) VALUES (?,?,?)");
            auto i_af   = i.af();
            i.addBindValue(id);
            i.addBindValue(base);
            i.addBindValue(hops);
            i.batch();
        }
        if(!d_targets.removed.empty()){
            QVariantList    id, base;
            
            for(auto i : d_targets.removed){
                id << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CTargets WHERE id=? AND tgt=?");
            auto d_af = d.af();
            d.addBindValue(id);
            d.addBindValue(base);
        }
        if(!d_targets.changed.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_targets.changed){
                int j   = targets[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery u(wksp::cache(), "UPDATE CTargets SET hops=? WHERE id=? AND tgt=?");
            auto u_af = u.af();
            
            u.addBindValue(hops);
            u.addBindValue(id);
            u.addBindValue(base);
            u.batch();
        }
        if(!d_targets.added.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_targets.added){
                int j   = targets[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }

            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CTargets (id,tgt,hops) VALUES (?,?,?)");
            auto i_af   = i.af();
            i.addBindValue(id);
            i.addBindValue(base);
            i.addBindValue(hops);
            i.batch();
        }
        if(!d_reverse.removed.empty()){
            QVariantList    id, base;
            
            for(auto i : d_reverse.removed){
                id << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CReverses WHERE id=? AND rev=?");
            auto d_af = d.af();
            d.addBindValue(id);
            d.addBindValue(base);
        }
        if(!d_reverse.changed.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_reverse.changed){
                int j   = reverse[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery u(wksp::cache(), "UPDATE CReverses SET hops=? WHERE id=? AND rev=?");
            auto u_af = u.af();
            
            u.addBindValue(hops);
            u.addBindValue(id);
            u.addBindValue(base);
            u.batch();
        }
        if(!d_reverse.added.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_reverse.added){
                int j   = reverse[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }

            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CReverses (id,rev,hops) VALUES (?,?,?)");
            auto i_af   = i.af();
            i.addBindValue(id);
            i.addBindValue(base);
            i.addBindValue(hops);
            i.batch();
        }
        if(!d_fields.removed.empty()){
            QVariantList    id, base;
            
            for(auto i : d_fields.removed){
                id << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CFields WHERE class=? AND field=?");
            auto d_af = d.af();
            d.addBindValue(id);
            d.addBindValue(base);
        }
        if(!d_fields.changed.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_fields.changed){
                int j   = fields[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }
        
            static thread_local SqlQuery u(wksp::cache(), "UPDATE CFields SET hops=? WHERE class=? AND field=?");
            auto u_af = u.af();
            
            u.addBindValue(hops);
            u.addBindValue(id);
            u.addBindValue(base);
            u.batch();
        }
        if(!d_fields.added.empty()){
            QVariantList        hops, id, base;
            for(auto i : d_fields.added){
                int j   = fields[i];
                hops << j;
                id  << (quint64) i.first.id;
                base << (quint64) i.second.id;
            }

            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CFields (class,field,hops) VALUES (?,?,?)");
            auto i_af   = i.af();
            i.addBindValue(id);
            i.addBindValue(base);
            i.addBindValue(hops);
            i.batch();
        }
        
        
        s_depends   = std::move(depends);
        s_sources   = std::move(sources);
        s_targets   = std::move(targets);
        s_reverse   = std::move(reverse);
        s_fields    = std::move(fields);

        if(doGraphs){
            if(!d_depends.empty())
                QThreadPool::globalInstance()->start(gen_class_dep);
        }
    }
    
    void        on_class_modify(Fragment fragment)
    {
        static const Folder  cfg = cdb::classes_folder();
        if(cdb::folder(fragment) != cfg){
            yWarning() << "Class document " << cdb::path(fragment) << " is not located in classes directory (ignored)";
            return ;
        }
        Document        doc = cdb::document(fragment);
        uClass& c = uget(cdb::db_class(doc));
        c.doc       = doc;
        on_interior_read(c);
        on_exterior_read(c);
        analyze_classes(true);
        //yDebug() << "Class " << c.cls.name() << " updated";        
    }
}

Graph       cur_class_dep_graph()
{
    return gClassDep;
}


void    init_class()
{
    Folder      cfg   = cdb::classes_folder();
    for(Document d : cdb::documents_by_suffix(cfg, "cls"))
        uget(cdb::db_class(d)).doc = d;
    for(Document d : cdb::documents_by_suffix_excluding(cfg, "cls"))
        yWarning() << "Class document " << cdb::key(d) << " is NOT located in classes directory (ignored)!";
    for(Class c : cdb::all_classes())
        on_interior_read(uget(c));
    for(Class c : cdb::all_classes())
        on_exterior_read(uget(c));
    analyze_classes(false);
    for(Class c : cdb::all_classes())
        create_db(uget(c));
    
    on_change({Change::Added, Change::Modified, Change::Removed}, "*.cls", on_class_modify);
}

void    init_class_graphs()
{
    QThreadPool::globalInstance()->start(gen_class_dep);
}

