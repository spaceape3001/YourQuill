////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uClass.hpp"
#include "uField.hpp"

#include "yCommon.hpp"
#include "yUpdater.hpp"

#include <db/AtomSys.hpp>
#include <db/Graph.hpp>
#include <db/GraphBuilder.hpp>
#include <db/Tag.hpp>
#include <db/Workspace.hpp>

#include <srv/Importer.hpp>
#include <util/Guarded.hpp>
#include <util/Hash.hpp>
#include <util/Logging.hpp>
#include <util/SqlQuery.hpp>
#include <util/SqlUtils.hpp>
#include <util/Utilities.hpp>

#include <QTextStream>
#include <QThreadPool>



UClass::UClass(Class c) : key(cdb::key(c)),  id(c.id), // cls(c), doc{c.id}, 
    folder(cdb::detail_folder(c)), edge(false)
{
}


Image               UClass::calc_icon() const
{
    Set<Class>  seen;
    Image   i   = explicit_icon();
    if(i)
        return i;
    seen << cls;
    
    Set<Class>   next, search;
    search      = use.def;
    search -= cls;
    while(!search.empty()){
        for(Class c : search){
            if(!seen.add(c))
                continue;
                
            auto& u = uget(c);
            i    = u.explicit_icon();
            if(i)
                return i;
            next += u.use.def;
        }
        search  = next - seen;
        next.clear();
    }
    
    return Image{};
}

Image               UClass::explicit_icon() const
{
    for(const char* x : Image::kSupportedExtensions){
        Document    d   = cdb::document(cdb::classes_folder(), key + "." + x);
        if(d && cdb::fragments_count(d))
            return cdb::image(d);
    }
    return Image{};
}


UClass&            uget(Class c)
{
    static Hash<uint64_t, UClass*>  data;   // yes, hash, because class IDs will not be continuous
    UClass* p   = data.get(c.id,nullptr);
    if(!p){
        p       = new UClass(c);
        data[c.id]  = p;
    }
    return *p;
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
    bool                                gClassInit      = false;
    
    
    QVariantList            qvar_first(const Set<CPair>& all)
    {
        QVariantList    ret;
        for(CPair v : all)
            ret << (quint64) v.first.id;
        return ret;
    }
    
    QVariantList            qvar_second(const Set<CPair>& all)
    {
        QVariantList    ret;
        for(CPair v : all)
            ret << (quint64) v.second.id;
        return ret;
    }

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
        if(add.empty() && rem.empty())
            return false;
        u.src.def = src;
        return true;
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
        if(add.empty() && rem.empty())
            return false;
        u.rev.def = rev;
        return true;
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
        if(add.empty() && rem.empty())
            return false;
        u.tags = tags;
        return true;
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
        if(add.empty() && rem.empty())
            return false;
        u.tgt.def   = tgt;
        return true;
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
            
            for(Class c2 : rem)
                uget(c2).der.def -= c.cls;
        }
        if(!add.empty()){
            static thread_local SqlQuery    i(wksp::cache(), "INSERT INTO CDependsDef (class,base) VALUES (?,?)");
            auto i_af = i.af();
            i.addBindValue((quint64) c.id);
            i.addBindValue(cdb::qvar_list(add));
            i.batch();
            
            for(Class c2 : add)
                uget(c2).der.def += c.cls;
        }
        
        if(add.empty() && rem.empty())
            return false;
        c.use.def   = use;
        return true;
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
        
        if(rem.empty() && add.empty())
            return false;
        u.aliases       = d.aliases;
        return true;
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
        
        if(add.empty() && rem.empty())
            return false;

        u.types = d.types;
        return true;
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
        
        if(add.empty() && rem.empty())
            return false;
        u.tags  = tags;
        return true;
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
        //u.icon          = icon_for(cdb::classes_folder(), u.key);
        
        static thread_local SqlQuery    u1(wksp::cache(), "UPDATE Classes SET name=?,plural=?,brief=?,removed=0 WHERE id=?");
        auto u1_af   = u1.af();
        u1.bind(0, u.data->name);
        u1.bind(1, u.data->plural);
        u1.bind(2, u.data->brief);
        u1.bind(3, u.id);
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
            //f.icon      = icon_for(u.folder, j.first.qString());
            f.implied   = false;
            
            String      name    = j.second.name.empty() ? j.first : j.second.name;
            String      plural  = j.second.plural.empty() ? (name + "s") : j.second.plural;
            String      pkey    = j.second.pkey.empty() ? (j.first + "s") : j.second.pkey;   
            
            
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

    void    analyze_edges()
    {
        for(Class c : cdb::all_classes()){
            UClass&u    = uget(c);
            
                //  EDGE FLAGS
            
            int         empty   = static_cast<int>(u.src.all.empty()) + static_cast<int>(u.tgt.all.empty());
            if(empty == 1){
                yWarning() << "Class " << u.key << " should have both sources and targets!";
            }
            
            u.edge      = empty != 2;
            static thread_local SqlQuery uc(wksp::cache(), "UPDATE Classes SET edge=? WHERE id=?");
            auto uc_af = uc.af();
            uc.bind(0, u.edge);
            uc.bind(1, u.id);
            uc.exec();
            
                //  CONNECTIONS

            Set<CPair>      cxn;
            for(Class s : u.src.all)
                for(Class t : u.tgt.all)
                    cxn << CPair(s,t);
            Set<CPair>      add = cxn - u.cxn;
            Set<CPair>      rem = u.cxn - cxn;
            
            if(!rem.empty()){
                static thread_local SqlQuery d(wksp::cache(), "DELETE FROM CEdges WHERE class=? AND source=? AND target=?");
                auto d_af = d.af();
                d.addBindValue((quint64) u.id);
                d.addBindValue(qvar_first(rem));
                d.addBindValue(qvar_second(rem));
                d.batch();
                
                for(auto& p : rem){
                    uget(p.first).out[p.second] -= c;
                    uget(p.second).in[p.first]  -= c;
                }
            }
            if(!add.empty()){
                static thread_local SqlQuery i(wksp::cache(), "INSERT INTO CEdges (class,source,target) VALUES (?,?,?)");
                auto i_af = i.af();
                i.addBindValue((quint64) u.id);
                i.addBindValue(qvar_first(add));
                i.addBindValue(qvar_second(add));
                i.batch();
                
                for(auto& p : add){
                    uget(p.first).out[p.second] += c;
                    uget(p.second).in[p.first]  += c;
                }
            }
        }
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
    
    void    db_check()
    {
        for(Class cc : cdb::all_classes()){
            UClass& c   = uget(cc);
            
            for(Field ff : c.fields.def){
                UField& f   = uget(ff);
                f.db        = QString("A_%1_%2").arg(c.key).arg(f.key);
                f.dbv       = QString("V_%1_%2").arg(c.key).arg(f.key);
                if(!db_table_exists( f.db, wksp::cache())){
                    if(f.types.has("charname")){
                        SqlQuery(wksp::cache(), QString(
                            "CREATE TABLE %1 ( "
                                "leaf   INTEGER, "
                                "attr   VARCHAR(255) COLLATE NOCASE, "   // the user defined "ID" from the attribute's line
                                "no     INTEGER, "                       // The "number" for the particular attribute
                                "value  VARCHAR(255), "
                                "first  VARCHAR(255), "
                                "last   VARCHAR(255), "
                                "middle VARCHAR(255) "
                            ")"
                        ).arg(f.db)).exec();
                    } else {
                        SqlQuery(wksp::cache(), QString(
                            "CREATE TABLE %1 ( "
                                "leaf  INTEGER, "
                                "attr  VARCHAR(255) COLLATE NOCASE, "   // the user defined "ID" from the attribute's line
                                "no    INTEGER, "                       // The "number" for the particular attribute
                                "value VARCHAR(255)"
                            ")"
                        ).arg(f.db)).exec();
                    }
                }

                if(!db_table_exists( f.dbv, wksp::cache())){
                    SqlQuery(wksp::cache(), QString(
                        "CREATE TABLE %1 ( "
                            "value VARCHAR(255) COLLATE NOCASE UNIQUE, "
                            "brief VARCHAR(255)"
                        ")").arg(f.dbv)).exec();
                }
            }
            
            if(c.edge){
                c.db        = QString("E_%1").arg(c.key);
                if(!db_table_exists(c.db, wksp::cache())){
                    SqlQuery(wksp::cache(), QString(
                        "CREATE TABLE %1 ( "
                        "   edge     INTEGER, "
                        "   source   INTEGER, "
                        "   target   INTEGER, "
                        "   reverse  INTEGER, "
                        "   explicit INTEGER NOT NULL DEFAULT 0 "
                        ")"
                    ).arg(c.db)).exec();
                }
            }
        }
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
        
        bool            wasCreated = false;
        Document        doc = cdb::document(fragment);
        Class           cc  = cdb::db_class(doc, &wasCreated);
        UChanged        chg = on_read(uget(cc));
        if(chg.use || wasCreated){
            analyze_use();
            chg.src     = true;
            chg.tgt     = true;
            chg.rev     = true;
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
        if(chg.use || chg.tgt || chg.src)
            analyze_edges();
        db_check();

        if(chg.use)
            launch_graphs();
        //yDebug() << "Class " << c.cls.name() << " updated";        
    }
}

Graph       cur_class_dep_graph()
{
    return gClassDepGraph;
}

#if 0
Image       calc_icon_for(Class cls)
{
    Set<Class>  seen;
    
    Image   i   = icon_of(cls);
    if(i)
        return i;
    seen << cls;
    
    Set<Class>   next, search;
    search      = makeSet(cdb::def_use(cls));
    search -= cls;
    while(!search.empty()){
        for(Class c : search){
            if(!seen.add(c))
                continue;
            i   = icon_of(c);
            if(i)
                return i;
            next += cdb::def_use(c);
        }
        search  = next - seen;
        next.clear();
    }
    
    return Image{};
}
#endif

void        update_icon(Class c)
{
    static thread_local SqlQuery u(wksp::cache(), "UPDATE Classes SET icon=? WHERE id=?");
    auto u_af = u.af();
    u.bind(0, uget(c).calc_icon().id);
    u.bind(1, c.id);
    u.exec();
}

void        update_class_icon(Fragment frag)
{
    QString     ckey    = cdb::skeyb(frag);

    Class   cls   = cdb::class_(ckey);
    if(!cls)
        return ;
    update_icon(cls);
    for(Class d : cdb::dependents(cls)){
        if(d != cls)
            update_icon(d);
    }
}

void        update_icon(Field f)
{
}


void        update_field_icon(Fragment frag)
{
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
    analyze_edges();
    db_check();
    
    for(Class c : cdb::all_classes()){
        update_icon(c);
        for(Field f : cdb::fields(c))
            update_icon(f);
    }
    
    on_change({Change::Added, Change::Modified, Change::Removed}, "*.cls", on_class_modify);
    gClassInit      = true;
}

void    init_class_graphs()
{
    launch_graphs();
}

