////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <0/basic/CollectionUtils.hpp>


#include <yq/mithril/category/CategoryCDB.hpp>
#include <yq/mithril/class/ClassCDB.hpp>
#include <yq/mithril/class/ClassUpdate.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>
#include <yq/mithril/field/FieldCDB.hpp>
#include <yq/mithril/field/FieldUpdate.hpp>
#include <yq/mithril/file/FileSpec.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/tag/TagCDB.hpp>
#include <yq/mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    std::pair<UClass&, bool>  UClass::create(Document doc)
    {
        bool created = false;
        Class    x   = cdb::db_class(doc,&created);
        return { get(x), created };
    }

    UClass&  UClass::get(Class x)
    {
        return U<Class>::lookup<UClass>(x);
    }

    void    UClass::icons(Fragment frag, [[maybe_unused]] Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::icons('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif

        Class    x   = cdb::class_(cdb::document(frag), true);
        if(!x)
            return ;
        get(x).u_icon();
    }

    const FileSpec&  UClass::lookup()
    {
        static const FileSpec   s_ret(CACHE, cdb::classes_folder(), "*.class");
        return s_ret;
    }


    void    UClass::notify(Fragment frag,Change chg)
    {
        on_notify(frag, chg);
    }
    
    Flags<What>    UClass::on_notify(Fragment frag,Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::notify('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif
    
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Class    x   = cdb::class_(doc);
                if(x){
                    get(x).x_erase();
                }
                return What::REMOVED;
            }
        }
        
        auto [u,cr] = create(doc);

        u.reload();
        u.u_info();
        u.u_tags();
        u.u_alias();
        if(cr)
            u.u_icon();

        return {};
    }

    void    UClass::s3(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::s3('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_icon();
        u.u_tags();
        u.u_alias();
    }
    
    void    UClass::s3_bind(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::s3_bind('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.i0_uses();
        u.i0_sources();
        u.i0_targets();
        u.i0_reverses();
    }

    void    UClass::s3_extend(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::s3_extend('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.u_bases();
    }
    
    void    UClass::s3_derives(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::s3_derives('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.u_derives();
    }

    void    UClass::s3_deduce(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::s3_deduce('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.u_fields();
        u.u_sources();
        u.u_targets();
        u.u_edge();
        u.u_reverses();
    }
    
    void    UClass::s3_propagate(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UClass::s3_propagate('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.u_inbounds();
        u.u_outbounds();
        u.u_resolves();
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


    void            UClass::enum_base(ClassHopMap& ret, Class c, hop_t d)
    {
        auto    j   = ret.find(c);
        if((j != ret.end()) && (j->second <= d))
            return ;
        j->second       = {d};
        for(Class cc : get(c).bases.direct){
            if(cc == c)
                continue;
            enum_base(ret, cc, d+1);
        }
    }

    ClassHopMap     UClass::enum_bases() const
    {
        ClassHopMap ret;
        if(x){
            for(Class c : bases.direct)
                enum_base(ret, c, 0);
        }
        return ret;
    }

    FieldHopMap     UClass::enum_fields() const
    {
        FieldHopMap ret;
        if(x){
            for(Field f : fields.direct)
                ret[f]  = { 0 };
            for(auto& j : bases.hop){
                UClass& u   = get(j.first);
                HopCount    hc  = j.second;
                ++hc;
                for(Field f : u.fields.direct)
                    ret[f].set_smaller(hc);
            }
            for(Field f : fields.all)
                ret.try_emplace(f, HopCount( -1 ));
        }
        return ret;
    }

    ClassHopMap UClass::enum_reverses() const
    {
        ClassHopMap ret;
        if(x){
            for(Class c : reverses.direct){
                ret[c]  = { 0 };
                UClass&uu   = get(c);
                for(Class d : uu.derives.direct)    // all their derives
                    ret[d].set_smaller(HopCount(1));
            }
                
            #if 0
            for(auto& j : bases.hop){                   // our base classes
                UClass& u   = get(j.first);
                HopCount    hc  = j.second;
                ++hc;
                for(Class c : u.reverses.direct){    // their targets
                    ret[c].set_smaller(hc);
                    UClass&uu   = get(c);
                    HopCount    hd = hc;
                    ++hd;
                    for(Class d : uu.derives.direct)    // and all their derives
                        ret[d].set_smaller(hd);
                }
            }
            #endif

            #if 1
            for(auto& j : derives.hop){                   // our base classes
                UClass& u   = get(j.first);
                HopCount    hc  = j.second;
                ++hc;
                for(Class c : u.reverses.direct){    // their targets
                    ret[c].set_smaller(hc);
                    UClass&uu   = get(c);
                    HopCount    hd = hc;
                    ++hd;
                    for(Class d : uu.derives.direct)    // and all their derives
                        ret[d].set_smaller(hd);
                }
            }
            #endif
        }
        return ret;
    }

    ClassHopMap UClass::enum_sources() const
    {
        ClassHopMap ret;
        if(x){
            for(Class c : sources.direct){
                ret[c]  = { 0 };
                UClass&uu   = get(c);
                for(Class d : uu.derives.direct)    // all their derives
                    ret[d].set_smaller(HopCount(1));
            }
                
            #if 1
            for(auto& j : bases.hop){                   // our base classes
                UClass& u   = get(j.first);
                HopCount    hc  = j.second;
                ++hc;
                for(Class c : u.sources.direct){    // their targets
                    ret[c].set_smaller(hc);
                    UClass&uu   = get(c);
                    HopCount    hd = hc;
                    ++hd;
                    for(Class d : uu.derives.direct)    // and all their derives
                        ret[d].set_smaller(hd);
                }
            }
            #endif

            #if 0
            for(auto& j : derives.hop){                   // our base classes
                UClass& u   = get(j.first);
                HopCount    hc  = j.second;
                ++hc;
                for(Class c : u.sources.direct){    // their targets
                    ret[c].set_smaller(hc);
                    UClass&uu   = get(c);
                    HopCount    hd = hc;
                    ++hd;
                    for(Class d : uu.derives.direct)    // and all their derives
                        ret[d].set_smaller(hd);
                }
            }
            #endif
        }
        return ret;
    }

    ClassHopMap UClass::enum_targets() const
    {
        ClassHopMap ret;
        if(x){
            for(Class c : targets.direct){
                ret[c]  = { 0 };
                UClass&uu   = get(c);
                for(Class d : uu.derives.direct)    // all their derives
                    ret[d].set_smaller(HopCount(1));
            }
            
            #if 1
            for(auto& j : bases.hop){                   // our base classes
                UClass& u   = get(j.first);
                HopCount    hc  = j.second;
                ++hc;
                for(Class c : u.targets.direct){    // their targets
                    ret[c].set_smaller(hc);
                    UClass&uu   = get(c);
                    HopCount    hd = hc;
                    ++hd;
                    for(Class d : uu.derives.direct)    // and all their derives
                        ret[d].set_smaller(hd);
                }
            }
            #endif

            #if 0
            for(auto& j : derives.hop){                   // our base classes
                UClass& u   = get(j.first);
                HopCount    hc  = j.second;
                ++hc;
                for(Class c : u.targets.direct){    // their targets
                    ret[c].set_smaller(hc);
                    UClass&uu   = get(c);
                    HopCount    hd = hc;
                    ++hd;
                    for(Class d : uu.derives.direct)    // and all their derives
                        ret[d].set_smaller(hd);
                }
            }
            #endif
        }
        return ret;
    }

    void    UClass::reload()
    {
        def     = cdb::merged(x, cdb::DONT_LOCK|cdb::IS_UPDATE);
        if(def){
            binding = def->binding;
        } else {
            binding = {};
        }
    }

    void    UClass::i0_reverses()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        for(std::string_view ck : def->reverse){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable REVERSE " << ck << " referenced in class " << key;
                continue;
            }
        
            reverses.direct.insert(y);
            get(y).reverses.direct.insert(y);
        }
    }

    void    UClass::i0_sources()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        for(std::string_view ck : def->sources){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable SOURCE " << ck << " referenced in class " << key;
                continue;
            }
        
            sources.direct.insert(y);
            get(y).outbounds.direct.insert(x);
        }
    }

    void    UClass::i0_targets()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        for(std::string_view ck : def->targets){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable TARGET " << ck << " referenced in class " << key;
                continue;
            }
            
            targets.direct.insert(y);
            get(y).inbounds.direct.insert(x);
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
    
            bases.direct.insert(y);
            get(y).derives.direct.insert(x);
        }
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

    void    UClass::u_bases()
    {
        if(!x)
            return ;
            
        ClassHopMap them   = enum_bases();
        map_difference_exec(bases.hop, them, 
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery  sql("DELETE FROM Class$Depends WHERE class=? AND base=?");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("UPDATE Class$Depends SET hops=? WHERE class=? AND base=?");
                sql.bind(1, (int) j.second.cnt);
                sql.bind(2, id);
                sql.bind(3, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("INSERT INTO Class$Depends (class, base, hops) VALUES (?,?,?)");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.bind(3, (int) j.second.cnt);
                sql.exec();
            }
        );
        bases.hop   = them;
    }

    void    UClass::u_derives()
    {
        if(!x)
            return;
        for(auto& cr : cdb::derived_classes_ranked(x))
            derives.hop[cr.cls].set_smaller({ (hop_t) cr.rank });
    }

    void    UClass::u_edge()
    {
        if(!x)
            return ;
            
        isEdge  = !(sources.hop.empty() || targets.hop.empty());
        static thread_local CacheQuery sql("UPDATE Classes SET edge=? WHERE id=?");
        sql.bind(1, isEdge);
        sql.bind(2, id);
        sql.exec();
    }

    void    UClass::u_fields()
    {
        if(!x)
            return ;
        
        FieldHopMap them   = enum_fields();
        map_difference_exec(fields.hop, them, 
            [&](const FieldHopMap::value_type& j){
                //  deletes
                static thread_local CacheQuery sql("DELETE FROM CFields WHERE class=? AND field=?");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.exec();
            }, 
            [&](const FieldHopMap::value_type& j){
                //  modifications
                static thread_local CacheQuery sql("UPDATE CFields SET hops=? WHERE class=? AND field=?");
                sql.bind(1, (int) j.second.cnt);
                sql.bind(2, id);
                sql.bind(3, j.first.id);
                sql.exec();
            }, 
            [&](const FieldHopMap::value_type& j){
                //  inserts
                static thread_local CacheQuery sql("INSERT INTO CFields (field, class, hops) VALUES (?, ?, ?)");
                sql.bind(1, j.first.id);
                sql.bind(2, id);
                sql.bind(3, (int) j.second.cnt);
                sql.exec();
            }
        );
        fields.hop  = them;
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

    void    UClass::u_inbounds()
    {
        if(!x)
            return;
        for(auto& cr : cdb::inbound_classes_ranked(x))
            inbounds.hop[cr.cls].set_smaller({ (hop_t) cr.rank });
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

    void    UClass::u_outbounds()
    {
        if(!x)
            return;
        for(auto& cr : cdb::outbound_classes_ranked(x))
            outbounds.hop[cr.cls].set_smaller({ (hop_t) cr.rank });
    }

    void    UClass::u_resolves()
    {
        //  NODES
        for(Class c : cdb::all_classes(Sorted::YES)){
            auto& u     = get(c);
            if(!u.isEdge){
                resolve[u.key]  = NodeClass{ c };
                for(auto& a : u.aliases)
                    resolve[a]  = NodeClass{ c };
            }
        }
        
        //  EDGES
        for(auto& j : outbounds.hop){
            Class c = j.first;
            auto& u = get(c);
            resolve[u.key]      = OutClass{ c };
            for(auto& a : u.aliases)
                resolve[a]  = OutClass{ c };
        }
        
        //  FIELDS
        for(auto& j : fields.hop){
            Field f = j.first;
            auto& u     = UField::get(f);
            resolve[u.key]  = f;
            for(auto& a : u.aliases)
                resolve[a]  = f;
        }
    }

    void    UClass::u_reverses()
    {
        if(!x)
            return ;
            
        ClassHopMap     them    = enum_reverses();
        map_difference_exec(reverses.hop, them, 
            [&](const ClassHopMap::value_type& j){
                //  deletes
                static thread_local CacheQuery sql("DELETE FROM CReverses WHERE class=? AND reverse=?");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("UPDATE CReverses SET hops=? WHERE class=? AND reverse=?");
                sql.bind(1, (int) j.second.cnt);
                sql.bind(2, id);
                sql.bind(3, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("INSERT INTO CReverses (class, reverse, hops) VALUES (?,?,?)");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.bind(3, (int) j.second.cnt);
                sql.exec();
            }
        );
        reverses.hop = them;
    }

    void    UClass::u_sources()
    {
        if(!x)
            return ;
            
        ClassHopMap     them    = enum_sources();
        map_difference_exec(sources.hop, them, 
            [&](const ClassHopMap::value_type& j){
                //  deletes
                static thread_local CacheQuery sql("DELETE FROM CSources WHERE class=? AND source=?");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("UPDATE CSources SET hops=? WHERE class=? AND source=?");
                sql.bind(1, (int) j.second.cnt);
                sql.bind(2, id);
                sql.bind(3, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("INSERT INTO CSources (class, source, hops) VALUES (?,?,?)");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.bind(3, (int) j.second.cnt);
                sql.exec();
            }
        );
        sources.hop = them;
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

    void    UClass::u_targets()
    {
        if(!x)
            return ;
            
        ClassHopMap     them    = enum_targets();
        map_difference_exec(targets.hop, them, 
            [&](const ClassHopMap::value_type& j){
                //  deletes
                static thread_local CacheQuery sql("DELETE FROM CTargets (class, target) WHERE class=? AND target=?");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("UPDATE CTargets SET hops=? WHERE class=? AND target=?");
                sql.bind(1, (int) j.second.cnt);
                sql.bind(2, id);
                sql.bind(3, j.first.id);
                sql.exec();
            },
            [&](const ClassHopMap::value_type& j){
                static thread_local CacheQuery sql("INSERT INTO CTargets (class, target, hops) VALUES (?,?,?)");
                sql.bind(1, id);
                sql.bind(2, j.first.id);
                sql.bind(3, (int) j.second.cnt);
                sql.exec();
            }
        );
        targets.hop = them;
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
