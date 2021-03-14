////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yCommon.hpp"
#include "yUpdater.hpp"

#include <db/AtomSys.hpp>
#include <db/Graph.hpp>
#include <db/GraphBuilder.hpp>
#include <db/Tag.hpp>
#include <db/Workspace.hpp>

#include <srv/Importer.hpp>
#include <util/Guarded.hpp>
#include <util/Logging.hpp>
#include <util/SqlQuery.hpp>
#include <util/SqlUtils.hpp>
#include <util/Utilities.hpp>

#include <QTextStream>
#include <QThreadPool>


UClass::UClass(Class c) : key(cdb::key(c)),  id(c.id), // cls(c), doc{c.id}, 
    folder(cdb::config_folder(c))
{
    ////  Add ourselves to ourselves ... 
        //use.all += c;
        //der.all += c;
}


UField::UField(Field f) : key(cdb::key(f)), id(f.id), implied(false)
{
}

/*
    Quick notes: want auto edges to commence .... that's the WHOLE point of this..
    
    Roster ... auto back (typcially role name, or simply "name")
    
    If a leaf has an atom attribute ... if it's a non-edge, will create a (default) edge and instantiate the atom
    
    Allow edges to be defined in both ways
    
    Birth/Death become inherent events.... auto-sub will allow too.

    Auto-sub-atom into the explorer.

    Need to auto-sub-atom things too.
*/

namespace {
    using CPair = std::pair<Class,Class>;
    using FPair = std::pair<Class,Field>;

        //  Class depenedency generator
    struct ClassDependencies {
        Vector<Class>               all;
        Vector<CPair>               pairs;
        Map<Class,Set<Class>>       uses, deriveds;
        
        ClassDependencies()
        {
            all = cdb::all_classes();
            for(Class c : all)
                for(Class b : uget(c).use.def)
            {
                pairs.push_back({ c, b });
                uses[c] << b;
                deriveds[b] << c;
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
        
        using Ptr   = std::shared_ptr<ClassDependencies>;
        
    };


    Guarded<Graph>                      gClassDepGraph;
    Guarded<ClassDependencies::Ptr>     gClassDepInfo;

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  READING of the classes from the drive
        
   

    struct UChanged {
        bool    use, rev, src, tgt, tags, fields;
    };
    
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  CLASS UPDATERS
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    
    bool        update_source(UClass&u)
    {
        ClassSet   src;
        for(const String& s : u.data->sources)
            src << cdb::db_class(s.qString());
        ClassSet    add        = src - u.src.def;
        ClassSet    rem        = u.src.def - src;
        if(!rem.empty()){
            static thread_local SqlQuery    d(wksp::cache(), "DELETE FROM CSourcesDef WHERE class=? AND source=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(cdb::qvar_list(rem));
            d.batch();
        }
        if(!add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO CSourcesDef (class,source) VALUES (?,?)");
            auto i_af = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(cdb::qvar_list(add));
            i.batch();
        }
        std::swap(u.src.def, src);
        return u.src.def != src;
    }
    

    bool    update_reverse(UClass&u)
    {
        ClassSet    rev;
        for(const String& s : u.data->reverse)
            rev << cdb::db_class(s.qString());
        ClassSet    add        = rev - u.rev.def;
        ClassSet    rem        = u.rev.def - rev;
        if(!rem.empty()){
            static thread_local SqlQuery    d(wksp::cache(), "DELETE FROM CReversesDef WHERE class=? AND reverse=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(cdb::qvar_list(rem));
            d.batch();
        }
        if(!add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO CReversesDef (class,reverse) VALUES (?,?)");
            auto i_af = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(cdb::qvar_list(add));
            i.batch();
        }
        std::swap(rev, u.rev.def);
        return rev != u.rev.def;
    }
    
    bool    update_tags(UClass&u)
    {
        TagSet      tags;
        for(const String& s : u.data -> tags)
            tags << cdb::db_tag(s.qString());
        TagSet      add    = tags - u.tags;
        TagSet      rem    = u.tags - tags;
        if(!rem.empty()){
            static thread_local SqlQuery    d(wksp::cache(), "DELETE FROM CTags WHERE class=? AND tag=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(cdb::qvar_list(rem));
            d.batch();
        }
        if(!add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO CTags (class, tag) VALUES (?,?)");
            auto i_af = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(cdb::qvar_list(add));
            i.batch();
        }
        std::swap(tags, u.tags);
        return tags != u.tags;
    }
    
    bool    update_target(UClass& u)
    {
        ClassSet   tgt;
        for(const String& s : u.data->targets)
            tgt << cdb::db_class(s.qString());
        ClassSet    add        = tgt - u.tgt.def;
        ClassSet    rem        = u.tgt.def - tgt;
        if(!rem.empty()){
            static thread_local SqlQuery    d(wksp::cache(), "DELETE FROM CTargetsDef WHERE class=? AND target=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(cdb::qvar_list(rem));
            d.batch();
        }
        if(!add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO CTargetsDef (class,target) VALUES (?,?)");
            auto i_af = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(cdb::qvar_list(add));
            i.batch();
        }
        std::swap(tgt, u.tgt.def);
        return u.tgt.def != tgt;
    }

    bool        update_use(UClass&c)
    {
        ClassSet   use;
        for(const String& s : c.data->use)
            use << cdb::db_class(s.qString());
            
        ClassSet    add    = use - c.use.def;
        ClassSet    rem    = c.use.def - use;
        if(!rem.empty()){
            static thread_local SqlQuery    d(wksp::cache(), "DELETE FROM CDependsDef WHERE class=? AND base=?");
            auto d_af = d.af();
            d.addBindValue((quint64) c.id);
            d.addBindValue(cdb::qvar_list(rem));
            d.batch();
        }
        if(!add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO CDependsDef (class,base) VALUES (?,?)");
            auto i_af = i.af();
            i.addBindValue((quint64) c.id);
            i.addBindValue(cdb::qvar_list(add));
            i.batch();
        }
        std::swap(c.use.def, use);
        return  use != c.use.def;
    }
        
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  FIELD UPDATERS
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    bool    update_aliases(UField& u, const ClassData::Field& d)
    {
        StringSet   add        = d.aliases - u.aliases;
        StringSet   rem        = u.aliases - d.aliases;
        
        if(!rem.empty()){
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM FAlias WHERE field=? AND alias=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(qvar_list(rem));
            d.batch();
        }
        
        if(!add.empty()){
            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO FAlias (field, alias) VALUES (?, ?)");
            auto i_af = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(qvar_list(add));
            i.batch();
        }
        
        bool    changed = u.aliases != d.aliases;
        u.aliases       = d.aliases;
        return changed;
    }
    
    bool    update_data_types(UField& u, const ClassData::Field& d)
    {
        StringSet   add     = d.types - u.types;
        StringSet   rem     = u.types - d.types;
        
        if(!rem.empty()){
            static thread_local SqlQuery    d(wksp::cache(), "DELETE FROM FDataTypes WHERE field=? AND type=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(qvar_list(rem));
            d.batch();
        }
        
        if(!add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO FDataTypes (field, type) VALUES (?, ?)");
            auto i_af = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(qvar_list(add));
            i.batch();
        }
        
        bool    ret = u.types != d.types;
        u.types = d.types;
        return ret;
    }


    bool    update_tags(UField& u, const ClassData::Field& d)
    {
        TagSet  tags;
        for(const String& s : d.tags)
            tags << cdb::db_tag(s.qString());
        
        TagSet add    = tags - u.tags;
        TagSet rem    = u.tags - tags;
        
        if(!rem.empty()){
            static thread_local SqlQuery    d(wksp::cache(), "DELETE FROM FTags WHERE field=? AND tag=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(cdb::qvar_list(rem));
            d.batch();
        }
        
        if(add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO FTags (field, tag) VALUES (?, ?)");
            auto i_af = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(cdb::qvar_list(add));
            i.batch();
        }
        std::swap(tags, u.tags);
        return tags != u.tags;
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
        //! Updates class data that does NOT refer to another class
    UChanged    on_read(UClass& u)
    {
        UChanged    ret{};
        if(cdb::fragments_count(u.doc) == 0)
            return ret;
        
        u.data          = cdb::merged(u.cls);
        u.icon          = icon_for(cdb::classes_folder(), u.key);
        
        static thread_local SqlQuery    u1(wksp::cache(), "UPDATE Classes SET name=?,plural=?,brief=?,icon=?,removed=0 WHERE id=?");
        auto u1_af   = u1.af();
        u1.bind(0, u.data->name);
        u1.bind(1, u.data->plural);
        u1.bind(2, u.data->brief);
        u1.bind(3, u.icon.id);
        u1.bind(4, u.id);
        u1.exec();

        ret.use     = update_use(u);
        ret.src     = update_source(u);
        ret.tgt     = update_target(u);
        ret.rev     = update_reverse(u);
        ret.tags    = update_tags(u);
        
        FieldSet    fields;
        for(auto& j : u.data->fields){
            UField& f   = uget(cdb::db_field(u.cls, j.first.qString()));
            f.cls       = u.cls;
            f.icon      = icon_for(u.folder, j.first.qString());
            f.implied   = false;
            
            String      name    = j.second.name.empty() ? j.first : j.second.name;
            String      plural  = j.second.plural.empty() ? (name + "s") : j.second.plural;
            String      pkey    = j.second.pkey.empty() ? (j.first + "s") : j.second.pkey;   
            f.db        = QString("Attr_%1_%2").arg(u.key).arg(f.key);
            f.dbv       = QString("Values_%1_%2").arg(u.key).arg(f.key);
            
            if(!db_table_exists( f.db, wksp::cache())){
                SqlQuery(wksp::cache(), QString(
                    "CREATE TABLE %1 ( "
                        "leaf INTEGER, "
                        "attr VARCHAR(255) COLLATE NOCASE, "
                        "value VARCHAR(255)"
                    ")").arg(f.db)).exec();
            }

            if(!db_table_exists( f.dbv, wksp::cache())){
                SqlQuery(wksp::cache(), QString(
                    "CREATE TABLE %1 ( "
                        "value VARCHAR(255) COLLATE NOCASE UNIQUE, "
                        "brief VARCHAR(255)"
                    ")").arg(f.dbv)).exec();
            }
            
            static thread_local SqlQuery    u2(wksp::cache(), 
                "UPDATE Fields SET name=?,pkey=?,plural=?,brief=?,multi=?,restrict=?,category=?,dbt=?,dbv=?,max=?,removed=0 WHERE id=?"
            );
            auto u2_af  = u2.af();
            
            u2.bind(0, name.qString());
            u2.bind(1, pkey.qString());
            u2.bind(2, plural.qString());
            u2.bind(3, j.second.brief.qString());
            u2.bind(4, j.second.multiplicity.key().qString());
            u2.bind(5, j.second.restriction.key().qString());
            u2.bind(6, j.second.category.qString());
            u2.bind(7, f.db);
            u2.bind(8, f.dbv);
            u2.bind(9, (quint64) j.second.max_count);
            u2.bind(10, f.id);
            u2.exec();
            
            update_tags(f, j.second);
            update_data_types(f, j.second);
            update_aliases(f, j.second);
            
            
            f.atoms.def.clear();
            for(auto & s : j.second.atoms)
                f.atoms.def << cdb::db_class(s.qString());
            fields << f.field;
        }
        
        FieldSet    fadd    = fields - u.fields.def;
        FieldSet    frem    = u.fields.def - fields;
        ret.fields    = u.fields.def != fields;
        std::swap(fields, u.fields.def);
        if(!frem.empty()){
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CFieldsDef WHERE class=? AND field=?");
            auto d_af = d.af();
            d.addBindValue((quint64) u.id);
            d.addBindValue(cdb::qvar_list(frem));
            d.batch();
        }
        if(!fadd.empty()){
            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CFieldsDef (class,field) VALUES (?,?)");
            auto i_af   = i.af();
            i.addBindValue((quint64) u.id);
            i.addBindValue(cdb::qvar_list(fadd));
            i.batch();
        }
        
        return ret;
    }

    
    bool    analyze_fields()
    {
        bool    changed = false;
        for(Class c : cdb::all_classes()){
            auto&       u     = uget(c);
            FieldSet    tmp = u.fields.def;
            for(Class b : u.use.all)
                tmp += uget(b).fields.def;
            FieldSet    add = tmp - u.fields.all;
            FieldSet    rem = u.fields.all - tmp;
            std::swap(tmp, u.fields.all);
            
            if(!rem.empty()){
                changed = true;
                
                static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CFields WHERE class=? AND field=?");
                auto d_af   = d.af();
                d.addBindValue((quint64) c.id);
                d.addBindValue(cdb::qvar_list(rem));
                d.batch();
            }

            if(!add.empty()){
                changed = true;
                
                static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CFields (class, field) VALUES (?, ?)");
                auto i_af   = i.af();
                i.addBindValue((quint64) c.id);
                i.addBindValue(cdb::qvar_list(add));
                i.batch();
            } 
        }
        return changed;
    }
    
    void    analyze_reverses()
    {
        for(Class c : cdb::all_classes()){
            auto&         u = uget(c);
            
            u.reverses      = u.rev.def;
    
            if(u.reverses.empty()){
                for(Class b : u.use.def)
                    u.reverses  += uget(b).rev.def;
            }
            
            ClassSet tmp    = u.reverses;
            for(Class t : u.reverses)
                tmp        += uget(t).der.all;

            ClassSet    add = tmp - u.rev.all;
            ClassSet    rem = u.rev.all - tmp;
            std::swap(tmp, u.rev.all);
            
            if(!rem.empty()){
                static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CReverses WHERE class=? AND reverse=?");
                auto d_af   = d.af();
                d.addBindValue((quint64) c.id);
                d.addBindValue(cdb::qvar_list(rem));
                d.batch();
            }
            
            
            if(!add.empty()){
                static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CReverses (class, reverse) VALUES (?, ?)");
                auto i_af   = i.af();
                i.addBindValue((quint64) c.id);
                i.addBindValue(cdb::qvar_list(add));
                i.batch();
            } 
        }
    }

    void    analyze_sources()
    {
        for(Class c : cdb::all_classes()){
            auto&         u = uget(c);
            
            u.sources       = u.src.def;
    
            if(u.sources.empty()){
                for(Class b : u.use.def)
                    u.sources += uget(b).src.def;
            }
            
            ClassSet tmp    = u.sources;
            for(Class t : u.sources)
                tmp        += uget(t).der.all;

            ClassSet    add = tmp - u.src.all;
            ClassSet    rem = u.src.all - tmp;
            std::swap(tmp, u.src.all);
            
            if(!rem.empty()){
                static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CSources WHERE class=? AND source=?");
                auto d_af   = d.af();
                d.addBindValue((quint64) c.id);
                d.addBindValue(cdb::qvar_list(rem));
                d.batch();
            }
            
            
            if(!add.empty()){
                static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CSources (class, source) VALUES (?, ?)");
                auto i_af   = i.af();
                i.addBindValue((quint64) c.id);
                i.addBindValue(cdb::qvar_list(add));
                i.batch();
            } 
        }
    }
    
    void    analyze_targets()
    {
        for(Class c : cdb::all_classes()){
            auto&         u = uget(c);
            
            u.targets       = u.tgt.def;
    
            if(u.targets.empty()){
                for(Class b : u.use.def)
                    u.targets  += uget(b).tgt.def;
            }
            
            ClassSet tmp    = u.targets;
            for(Class t : u.targets)
                tmp        += uget(t).der.all;

            ClassSet    add = tmp - u.tgt.all;
            ClassSet    rem = u.tgt.all - tmp;
            std::swap(tmp, u.tgt.all);
            
            if(!rem.empty()){
                static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CTargets WHERE class=? AND target=?");
                auto d_af   = d.af();
                d.addBindValue((quint64) c.id);
                d.addBindValue(cdb::qvar_list(rem));
                d.batch();
            }
            
            
            if(!add.empty()){
                static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CTargets (class, target) VALUES (?, ?)");
                auto i_af   = i.af();
                i.addBindValue((quint64) c.id);
                i.addBindValue(cdb::qvar_list(add));
                i.batch();
            } 
        }
    }

    void    analyze_dependencies(Set<CPair>& deps, Class c1, const Set<Class>& use)
    {
        for(Class c2 : use){
            if(c1 == c2)                // catch circular dependencies
                continue;
            if(deps.add(CPair(c1,c2)))
                analyze_dependencies(deps, c1, uget(c2).use.def);
        }
    }


        //! \return TRUE if the any USE has changed....
    bool    analyze_use()   
    {
        static Set<CPair>       last;
        Set<CPair>              cur;

        for(Class c : cdb::all_classes())
            analyze_dependencies(cur, c, uget(c).use.def);

        Set<CPair>          add = cur - last;
        Set<CPair>          rem = last - cur;
        
        bool        changed     = false;
        
        if(!rem.empty()){
            changed         = true;
            QVariantList    id, base;
            
            for(auto i : rem){
                id << (quint64) i.first.id;
                base << (quint64) i.second.id;
                uget(i.first).use.all -= i.second;
                uget(i.second).der.all -= i.first;
            }
        
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CDepends WHERE class=? AND base=?");
            auto d_af = d.af();
            d.addBindValue(id);
            d.addBindValue(base);
            d.batch();
        }
        
        if(!add.empty()){
            changed         = true;
            QVariantList    id, base;
            for(auto i : add){
                id   << (quint64) i.first.id;
                base << (quint64) i.second.id;
                uget(i.first).use.all += i.second;
                uget(i.second).use.all += i.first;
            }

            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CDepends (class,base) VALUES (?,?)");
            auto i_af   = i.af();
            i.addBindValue(id);
            i.addBindValue(base);
            i.batch();
        }
        
        std::swap(cur, last);
        return changed;
    }
    

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  GRAPHS
    

    Graph   draw_class_graph(const Vector<Class>& classes, const Vector<CPair>& edges, Class focus, const QString& url /* assumes class id to be appended&cls=? will */ )
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


    

    void    gen_class_dep()
    {
        static thread_local SqlQuery u(wksp::cache(), "UPDATE Classes SET deps=? WHERE id=?");
        ClassDependencies::Ptr  cpp = gClassDepInfo;
        if(!cpp)
            return;
        
        gClassDepGraph   = draw_class_graph(cpp->all, cpp->pairs, Class{}, "/wksp/class?cls=");
        for(Class c : cpp->all){
            Vector<Class>       subC;
            Vector<CPair>       subU;
            cpp->get(c, subC, subU, 1, 3);
            
            Graph   g   = draw_class_graph(subC, subU, c, "/wksp/class/dependency?cls=");
            if(g){
                auto u_af   = u.af();
                u.bind(0, g.id);
                u.bind(1, c.id);
                u.exec();
            }
        }
    }

    void    launch_graphs()
    {   
        gClassDepInfo           = std::make_shared<ClassDependencies>();
        QThreadPool::globalInstance()->start(gen_class_dep);
    }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    void        on_class_modify(Fragment fragment)
    {
        if(cdb::folder(fragment) != cdb::classes_folder()){
            yWarning() << "Class document " << cdb::path(fragment) << " is not located in classes directory (ignored)";
            return ;
        }
        
        Document        doc = cdb::document(fragment);
        UChanged        chg = on_read(uget(cdb::db_class(doc)));

        if(chg.use){
            analyze_use();
            chg.src = true;
            chg.tgt = true;
            chg.rev    = true;
            chg.fields  = true;
        }
        
        if(chg.src)
            analyze_sources();
        if(chg.tgt)
            analyze_targets();
        if(chg.rev)
            analyze_reverses();
        if(chg.fields)
            analyze_fields();

        if(chg.use)
            launch_graphs();
        //yDebug() << "Class " << c.cls.name() << " updated";        
    }
}

Graph       cur_class_dep_graph()
{
    return gClassDepGraph;
}


void    init_class()
{
    for(Document d : cdb::documents_by_suffix_excluding(cdb::classes_folder(), "cls"))
        yWarning() << "Class document " << cdb::key(d) << " is NOT located in classes directory (ignored)!";
        
        //  Simply create the classes first
    for(Document d : cdb::documents_by_suffix(cdb::classes_folder(), "cls"))    
        cdb::db_class(d);
    for(Class c : cdb::all_classes())
        on_read(uget(c));
    
    analyze_use();
    analyze_sources();
    analyze_targets();
    analyze_reverses();
    analyze_fields();
    
    on_change({Change::Added, Change::Modified, Change::Removed}, "*.cls", on_class_modify);
}

void    init_class_graphs()
{
            launch_graphs();
}

