////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uClass.hpp"

#include <basic/CollectionUtils.hpp>
#include <basic/TextUtils.hpp>

#include <mithril/category/CategoryCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/tag/TagCDB.hpp>

namespace yq::mithril::update {
    UClass&  UClass::get(Class x)
    {
        return lookup<UClass>(x);
    }

    std::pair<UClass&, bool>  UClass::create(Document doc)
    {
        bool created = false;
        Class    x   = cdb::db_class(doc,&created);
        return { get(x), created };
    }

    void    UClass::icons(Fragment frag,Change)
    {
        Class    x   = cdb::class_(cdb::document(frag), true);
        if(!x)
            return ;
        get(x).u_icon();
    }

    void    UClass::notify(Fragment frag,Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Class    x   = cdb::class_(doc);
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
        if(cr)
            u.u_icon();
    }

    void    UClass::s3(Document doc)
    {
        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_icon();
        u.u_tags();
        u.u_alias();
    }
    
    void    UClass::s3_bind(Document doc)
    {
        auto [u,cr] = create(doc);
        u.i0_uses();
        u.i0_sources();
        u.i0_targets();
        u.i0_reverses();
    }

    void    UClass::s3_extend(Document doc)
    {
        auto [u,cr] = create(doc);
        u.i1_bases();
    }

    void    UClass::s3_deduce(Document doc)
    {
        auto [u,cr] = create(doc);
    }
    
    ////////////////////////////////////////////////////////////////////////////

    UClass::UClass(Class x) : U<Class>(x, cdb::key(x)), doc(cdb::document(x))
    {
    }

    string_set_t    UClass::enum_aliases() const
    {
        string_set_t    ret;
        if(def){
            for(auto& a : def->aliases)
                ret.insert(capitalize(a));
                
            if(!key.empty()){
                for(auto& p : def->prefixes)
                    ret.insert(capitalize(p+key));

                for(auto& p : def->prefixes)
                    for(auto& s : def->suffixes)
                        ret.insert(capitalize(p+key+s));
                
                for(auto& s : def->suffixes)
                    ret.insert(capitalize(key+s));
            }
                
                //  sample for generator:
                //  co_yield k+s;
            
            for(auto& aa : def->aliases)
                for(auto& p : def->prefixes)
                    ret.insert(capitalize(p+aa));
                    
            for(auto& aa : def->aliases)
                for(auto& p : def->prefixes)
                    for(auto& s : def->suffixes)
                        ret.insert(capitalize(p+aa+s));

            for(auto& aa : def->aliases)
                for(auto& s : def->suffixes)
                    ret.insert(capitalize(aa+s));
        }
        return ret;
    }

    void    UClass::reload()
    {
        def     = cdb::merged(x, cdb::DONT_LOCK|cdb::IS_UPDATE);
    }

    void    UClass::i0_reverses()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        static thread_local CacheQuery iReverse("INSERT INTO CReverses (class, reverse) VALUES (?,?)");

        for(std::string_view ck : def->reverse){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable REVERSE " << ck << " referenced in class " << key;
                continue;
            }
        
            iReverse.bind(1, id);
            iReverse.bind(2, y.id);
            iReverse.exec();
            
            reverses[y]         = { 0 };
            get(y).reverses[x]  = { 0 };
        }
    }

    void    UClass::i0_sources()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        static thread_local CacheQuery iSource("INSERT INTO CSources (class, source) VALUES (?,?)");

        for(std::string_view ck : def->sources){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable SOURCE " << ck << " referenced in class " << key;
                continue;
            }
        
            iSource.bind(1, id);
            iSource.bind(2, y.id);
            iSource.exec();
            
            sources[y]          = { 0 };
            get(y).outbound[x]  = { 0 };
        }
    }

    void    UClass::i0_targets()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        static thread_local CacheQuery iTarget("INSERT INTO CTargets (class, target) VALUES (?,?)");

        for(std::string_view ck : def->targets){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable TARGET " << ck << " referenced in class " << key;
                continue;
            }
        
            iTarget.bind(1, id);
            iTarget.bind(2, y.id);
            iTarget.exec();
            
            targets[y]          = { 0 };
            get(y).inbound[x]   = { 0 };
        }
    }
    
    void    UClass::i0_uses()
    {
        if(!x)
            return ;
        if(!def)
            return ;
        
        for(std::string_view ck : def->use){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable USE " << ck << " referenced in class " << key;
                continue;
            }
    
            use.insert(y);
        }
    }

    void    UClass::i1_bases(Class y,uint16_t d)
    {
        auto i = base.find(y);
        if((i != base.end()) && (i->second <= d))
            return ;
        
        UClass& u   = get(y);
    
        static thread_local CacheQuery iUse("INSERT OR REPLACE INTO CDepends (class, base, hops) VALUES (?,?,?)");
        iUse.bind(1, id);
        iUse.bind(2, y.id);
        iUse.bind(3, d);
        iUse.exec();
        
        base[y]     = { d };
        u.derive[x] = { d };
        
        for(Class c : u.use)
            i1_bases(c, d+1);
    }

    void    UClass::i1_bases()
    {
        if(!x)
            return ;
        if(!def)
            return ;
        for(Class c : use)
            i1_bases(c, 0);
    }

    void    UClass::u_alias()
    {
        if(!x)
            return ;
        if(!def)
            return ;
            
        static thread_local CacheQuery iAlias("INSERT INTO CAlias (class, alias) VALUES (?,?)");
        static thread_local CacheQuery dAlias("DELETE FROM CAlias WHERE class=? AND alias=?");

        string_set_t    them        = enum_aliases();
        auto chg    = add_remove(aliases, them);
        for(const std::string& s : chg.added)
            iAlias.exec(id, s);
        for(const std::string& s : chg.removed)
            dAlias.exec(id, s);
        aliases = them;
    }

    void    UClass::u_icon()
    {
        if(!x)
            return ;

        static thread_local CacheQuery u1("UPDATE Classes SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

        Image       img     = cdb::best_image(doc);
        if(img != icon){
                // we'll eventually do inheritance....
            u1.exec(img.id, id);
            u2.exec(doc.id, id);
            icon        = img;
        }
    }

    void    UClass::u_info()
    {
        if(!x)
            return ;
        if(!def)
            return ;
        
        Category    cat;
        if(!def->category.empty())
            cat = cdb::db_category(def->category);

        std::string_view    name = def->name;
        if(name.empty())
            name            = key;
            
        std::string         plural = def->plural;
        if(plural.empty())
            plural      = pluralize(name);

        static thread_local CacheQuery uInfo("UPDATE Classes SET name=?, plural=?, brief=?, category=?, binding=?, url=?, devurl=? WHERE id=?");
        uInfo.bind(1, name);
        uInfo.bind(2, plural);
        uInfo.bind(3, def->brief);
        uInfo.bind(4, cat.id);
        uInfo.bind(5, def->binding);
        uInfo.bind(6, def->url);
        uInfo.bind(7, def->dev_url);
        uInfo.bind(8, id);
        uInfo.exec();
    }

    void    UClass::u_tags()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        static thread_local CacheQuery iTag("INSERT INTO CTags (class, tag) VALUES (?,?)");
        static thread_local CacheQuery dTag("DELETE FROM CTags WHERE class=? AND tag=?");

        TagSet  new_tags = cdb::find_tags_set(def->tags, true);
        auto    chg     = add_remove(tags, new_tags);
        for(Tag t : chg.added)
            iTag.exec(x.id, t.id);
        for(Tag t : chg.removed)
            dTag.exec(x.id, t.id);
        tags    = new_tags;
    }

    void    UClass::x_erase()
    {
        static thread_local CacheQuery stmts[] = {
            CacheQuery("DELETE FROM CAlias WHERE class=?"),
            CacheQuery("DELETE FROM CPrefix WHERE class=?"),
            CacheQuery("DELETE FROM CSuffix WHERE class=?"),
            CacheQuery("DELETE FROM CTags WHERE class=?"),
            CacheQuery("DELETE FROM CLookup WHERE class=? AND priority=1"),
            CacheQuery("DELETE FROM Classes WHERE id=?")
        };
        
        if(!x)
            return ;
            
        for(auto& sq : stmts)
            sq.exec(id);
    }
}
