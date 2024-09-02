////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <0/basic/CollectionUtils.hpp>


#include <mithril/category/CategoryCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/field/FieldUpdate.hpp>
#include <mithril/file/FileSpec.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    std::pair<UField&, bool>  UField::create(Document doc)
    {
        bool created = false;
        Field    x   = cdb::db_field(doc,&created);
        return { get(x), created };
    }

    UField&  UField::get(Field x)
    {
        return U<Field>::lookup<UField>(x);
    }

    void    UField::icons(Fragment frag, [[maybe_unused]] Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UField::icons('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif

        Field    x   = cdb::field(cdb::document(frag), true);
        if(!x)
            return ;
        get(x).u_icon();
    }

    const FileSpec&  UField::lookup()
    {
        static const FileSpec s_ret(CACHE, cdb::fields_folder(), "*.field");
        return s_ret;
    }

    void    UField::notify(Fragment frag,Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UField::notify('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif

        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Field    x   = cdb::field(doc);
                if(x){
                    get(x).x_erase();
                }
                return ;
            }
        }
        
        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_tags();
        u.u_alias();
        u.u_dataTypes();
        u.u_atomTypes();
    }
    

    void     UField::s3(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UField::s3('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.reload();
        u.i_class();
        u.u_info();
        u.u_icon();
        u.u_tags();
        u.u_alias();
        u.u_dataTypes();
        u.u_atomTypes();
    }

    ////////////////////////////////////////////////////////////////////////////

    UField::UField(Field x) : U<Field>(x, cdb::key(x)), doc(cdb::document(x))
    {
        size_t  n   = key.find('.');
        if(n != std::string::npos){
            anycls  = false;
            clskey  = key.substr(0,n);
            subkey  = key.substr(n+1);
            cls     = cdb::db_class(clskey);
        } else {
            anycls  = true;
            subkey  = key;
        }
    }

    ClassHopMap UField::enum_atomTypes() const
    {
        ClassHopMap     ret;
        if(x && def){
            for(std::string_view s : def->atoms){
                Class   y   = cdb::db_class(s);
                ret[y]  = { 0 };
                UClass& u   = UClass::get(y);
                for(auto& d : u.derives.hop){
                    HopCount    hc  = d.second;
                    ++hc;
                    ret[d.first].set_smaller(hc);
                }
            }
        }
        return ret;
    }

    TypeInfoSet UField::enum_dataTypes() const
    {
        TypeInfoSet ret;
        if(def && x){
            for(std::string_view s : def->types){
                const TypeInfo* ti  = TypeInfo::find(s);
                if(!ti){
                    yInfo() << "Invalid data type '" << s << "' specified in field " << key;
                    continue;
                }
                
                ret.insert(ti);
            }
        }
        return ret;
    }

    void    UField::reload()
    {
        def     = cdb::merged(x, cdb::DONT_LOCK|cdb::IS_UPDATE);
    }

    void    UField::i_class()
    {
        if(!x)
            return ;
            
        static thread_local CacheQuery uStmt("UPDATE Fields SET class=?, ck=? WHERE id=?");
            
        if(anycls){
            for(Class c : cdb::all_classes(Sorted::YES)){
                auto& u = UClass::get(c);
                u.fields.all.insert(x);
            }
        } else {
            UClass::get(cls).fields.direct.insert(x);
            
            uStmt.bind(1, cls.id);
            uStmt.bind(2, subkey);
            uStmt.bind(3, id);
            uStmt.exec();
        }
    }

    void    UField::u_alias()
    {
        if(!x)
            return ;
        if(!def)
            return ;
             
        static thread_local CacheQuery iAlias("INSERT INTO FAlias (field, alias, usurp) VALUES (?,?,0)");
        static thread_local CacheQuery dAlias("DELETE FROM FAlias WHERE field=? AND alias=? AND usurp=0");

        auto chg    = add_remove(aliases, def->aliases);
        for(const std::string& s : chg.added)
            iAlias.exec(id, s);
        for(const std::string& s : chg.removed)
            dAlias.exec(id, s);
        aliases = def->aliases;
    }

    void    UField::u_atomTypes()
    {
        if(!x)
            return ;
        if(!def)
            return ;
    
        static thread_local CacheQuery iAtomType("INSERT INTO FAtomTypes (field, class, hops) VALUES (?, ?, ?)");
        static thread_local CacheQuery uAtomType("UPDATE FAtomTypes SET hops=? WHERE field=? AND class=?");
        static thread_local CacheQuery dAtomType("DELETE FROM FAtomTypes WHERE field=? AND class=?");

        ClassHopMap them    = enum_atomTypes();
        map_difference_exec(atomTypes, them, 
            [&](const ClassHopMap::value_type& j){
                    // delete
                dAtomType.bind(1, id);
                dAtomType.bind(2, j.first.id);
                dAtomType.exec();
            },
            [&](const ClassHopMap::value_type& j){
                    // modified
                uAtomType.bind(1, (int) j.second.cnt);
                uAtomType.bind(2, id);
                uAtomType.bind(3, j.first.id);
                uAtomType.exec();
            },
            [&](const ClassHopMap::value_type& j){
                    // insert
                iAtomType.bind(1, id);
                iAtomType.bind(2, j.first.id);
                iAtomType.bind(3, (int) j.second.cnt);
                iAtomType.exec();
            }
        );
        atomTypes   = them;
    }

    void    UField::u_dataTypes()
    {
        if(!x)
            return ;
        if(!def)
            return ;
            

        static thread_local CacheQuery iDataType("INSERT INTO FDataTypes (field, type) VALUES (?,?)");
        static thread_local CacheQuery dDataType("DELETE FROM FDataTypes WHERE field=? AND type=?");
        
        TypeInfoSet them    = enum_dataTypes();
        auto chg        = add_remove(dataTypes, them);
        for(const TypeInfo* ti : chg.added)
            iDataType.exec(id, ti->id());
        for(const TypeInfo* ti : chg.removed)
            dDataType.exec(id, ti->id());
        dataTypes = them;
    }

    void    UField::u_icon()
    {
        if(!x)
            return ;
            
        Image       img     = cdb::best_image(doc);
        
        static thread_local CacheQuery u1("UPDATE Fields SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

        if(img != icon){
            u1.exec(img.id, id);
            u2.exec(doc.id, id);
            icon    = img;
        }
    }

    void    UField::u_info()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        Category    cat;
        if(!def->category.empty())
            cat = cdb::db_category(def->category);
            
        std::string     name    = def->name;
        if(name.empty())
            name        = subkey;
            
        std::string     plural  = def->plural;
        if(plural.empty())
            plural      = pluralize(name);
        
        std::string     pkey    = def->pkey;
        if(pkey.empty()){
            pkey        = pluralize(subkey);
            pkey        = to_lower(pkey);
        }
    
        static thread_local CacheQuery uInfo("UPDATE Fields SET name=?, plural=?, pkey=?, expected=?, brief=?, multi=?, restrict=?, category=?, maxcnt=?, anycls=? WHERE id=?");
        uInfo.bind(1, name);
        uInfo.bind(2, plural);
        uInfo.bind(3, pkey);
        uInfo.bind(4, def->expected);
        uInfo.bind(5, def->brief);
        uInfo.bind(6, (int) def->multiplicity);
        uInfo.bind(7, (int) def->restriction);
        uInfo.bind(8, cat.id);
        uInfo.bind(9, def->max_count);
        uInfo.bind(10, anycls);
        uInfo.bind(11, id);
        uInfo.exec();
    }

    void    UField::u_tags()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        static thread_local CacheQuery iTag("INSERT INTO FTags (field, tag) VALUES (?,?)");
        static thread_local CacheQuery dTag("DELETE FROM FTags WHERE field=? AND tag=?");

        TagSet  new_tags = cdb::find_tags_set(def->tags, true);
        auto    chg     = add_remove(tags, new_tags);
        for(Tag t : chg.added)
            iTag.exec(x.id, t.id);
        for(Tag t : chg.removed)
            dTag.exec(x.id, t.id);
        tags    = new_tags;
    }

    void    UField::u_usurp()
    {
        if(!x)
            return ;
        if(!def)
            return ;
             
        static thread_local CacheQuery iUsurp("INSERT OR REPLACE INTO FAlias (field, alias, usurp) VALUES (?,?,1)");
        static thread_local CacheQuery dUsurp("DELETE FROM FAlias WHERE field=? AND alias=? AND usurp=1");

        auto chg    = add_remove(usurps, def->usurps);
        for(const std::string& s : chg.added)
            iUsurp.exec(id, s);
        for(const std::string& s : chg.removed)
            dUsurp.exec(id, s);
        usurps  = def->usurps;
    }
    
    void    UField::x_erase()
    {
        static thread_local CacheQuery stmts[] = {
            CacheQuery("DELETE FROM CFields WHERE field=?"),
            CacheQuery("DELETE FROM FTags WHERE field=?"),
            CacheQuery("DELETE FROM Fields WHERE id=?")
        };

        if(!x)
            return ;
        
        for(auto& sq : stmts)
            sq.exec(id);
    }
}

