////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/class/Class.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/class/ClassData.hpp>
#include <mithril/class/ClassDiff.hpp>
#include <mithril/class/ClassInfo.hpp>
#include <yq/container/map_utils.hpp>

namespace {
    void    class_graph_redraw(bool trigger=false)
    {
        static bool     s_ready = false;
        
        if(!s_ready){
            if(!trigger)
                return ;
                
            s_ready = true;
        }
        
            
    }
    
    template <typename T, typename I=hop_t>
    std::map<T, HCount<I>>   enumerate_hops(T c, std::function<std::set<T>(T)> fn)
    {
        std::map<T, HCount<I>>  ret;
        std::set<T>             checked, check;
        checked.insert(c);
        check = fn(c);
        
        for(I cnt=0;;++cnt){
            
            std::set<T> tmp  = check - checked;
            if(tmp.empty())    // usual exit
                break;

            check.clear();
            for(T cc : tmp){
                ret[cc] = { cnt };
                check += fn(cc);
            }
        }
        
        
        return ret;
    }
    

    void    u_class(Class c, Change chg, bool s5=false)
    {
        if(!c)
            return ;
    
        Class::Diff   x { 
            .x = c, 
            .id = c.id,
            .chg = chg, 
            .key = cdb::key(c)
        };
        
        Document doc    = cdb::document(c);
        
        if(chg != Change::Added){
            Class::Info     ii      = cdb::info(c);
            x.category.from         = ii.category;
            x.icon.from             = ii.icon;
            x.name.from             = std::move(ii.name);
            x.plural.from           = std::move(ii.plural);
            x.brief.from            = std::move(ii.brief);
            x.bases.from            = cdb::base_hops(c);
            x.deriveds.from         = cdb::derived_hops(c);
            x.tags.from             = cdb::tags_set(c);

            x.aliases.from          = cdb::def_alias_set(c);
            x.prefixes.from         = cdb::def_prefix_set(c);
            x.sources.from          = cdb::def_source_set(c);
            x.reverses.from         = cdb::def_reverse_set(c);
            x.suffixes.from         = cdb::def_suffix_set(c);
            x.targets.from          = cdb::def_target_set(c);
            x.uses.from             = cdb::def_use_set(c);
        }
        
        static thread_local CacheQuery iLookup("INSERT OR REPLACE INTO " TBL_CLASS_LOOKUP " (class, alias, priority) VALUES (?,?,0)");
        static thread_local CacheQuery iBase("INSERT OR REPLACE INTO " TBL_CLASS_DEPEND " (class, base, hops) VALUES (?,?,?)");
        static thread_local CacheQuery dBase("DELETE FROM " TBL_CLASS_DEPEND " WHERE class=? AND base=?");
        static thread_local CacheQuery uBase("UPDATE " TBL_CLASS_DEPEND " SET hops=? WHERE class=? AND base=?");
        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery uInfo("UPDATE " TBL_CLASSES " SET name=?, plural=?, brief=?, category=?, binding=?, url=?, devurl=?, icon=? WHERE id=?");
        static thread_local CacheQuery iTag("INSERT INTO " TBL_CLASS_TAG " (class, tag) VALUES (?,?)");
        static thread_local CacheQuery dTag("DELETE FROM " TBL_CLASS_TAG " WHERE class=? AND tag=?");
        
        static thread_local CacheQuery iDefAlias("INSERT INTO " TBL_CLSDEF_ALIAS " (class, alias) VALUES (?,?)");
        static thread_local CacheQuery dDefAlias("DELETE FROM " TBL_CLSDEF_ALIAS " WHERE class=? AND alias=?");
        static thread_local CacheQuery iDefPrefix("INSERT INTO " TBL_CLSDEF_PREFIX " (class, prefix) VALUES (?,?)");
        static thread_local CacheQuery dDefPrefix("DELETE FROM " TBL_CLSDEF_PREFIX " WHERE class=? AND prefix=?");
        static thread_local CacheQuery iDefReverse("INSERT INTO " TBL_CLSDEF_REVERSE " (class, reverse) VALUES (?,?)");
        static thread_local CacheQuery dDefReverse("DELETE FROM " TBL_CLSDEF_REVERSE " WHERE class=? AND reverse=?");
        static thread_local CacheQuery iDefSource("INSERT INTO " TBL_CLSDEF_SOURCE " (class, source) VALUES (?,?)");
        static thread_local CacheQuery dDefSource("DELETE FROM " TBL_CLSDEF_SOURCE " WHERE class=? AND source=?");
        static thread_local CacheQuery iDefSuffix("INSERT INTO " TBL_CLSDEF_SUFFIX " (class, suffix) VALUES (?,?)");
        static thread_local CacheQuery dDefSuffix("DELETE FROM " TBL_CLSDEF_SUFFIX " WHERE class=? AND suffix=?");
        static thread_local CacheQuery iDefTarget("INSERT INTO " TBL_CLSDEF_TARGET " (class, target) VALUES (?,?)");
        static thread_local CacheQuery dDefTarget("DELETE FROM " TBL_CLSDEF_TARGET " WHERE class=? AND target=?");
        static thread_local CacheQuery iDefUse("INSERT INTO " TBL_CLSDEF_USE " (class, use) VALUES (?,?)");
        static thread_local CacheQuery dDefUse("DELETE FROM " TBL_CLSDEF_USE " WHERE class=? AND use=?");

        if(chg != Change::Removed){
        
            iLookup.exec(x.id, x.key);
        
            auto def        = cdb::merged(c, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Class " << x.key << " cannot be read.";
                return;
            }

            x.icon.to               = cdb::best_image(doc);
            x.category.to           = cdb::category(def->category);
            x.name.to               = std::move(def->name);
            if(x.name.to.empty())
                x.name.to           = x.key;
            x.plural.to             = std::move(def->plural);
            x.brief.to              = std::move(def->brief);
            x.tags.to               = make_set(cdb::db_tags(def->tags));
            
            x.aliases.to            = std::move(def->aliases);
            x.prefixes.to           = std::move(def->prefixes);
            x.reverses.to           = make_set(cdb::db_classes(def->reverse));
            x.sources.to            = make_set(cdb::db_classes(def->sources));
            x.suffixes.to           = std::move(def->suffixes);
            x.targets.to            = make_set(cdb::db_classes(def->targets));
            x.uses.to               = make_set(cdb::db_classes(def->use));
            
            uInfo.bind(1, x.name.to);
            uInfo.bind(2, x.plural.to);
            uInfo.bind(3, x.brief.to);
            uInfo.bind(4, x.category.to);
            uInfo.bind(5, def->binding);
            uInfo.bind(6, def->url);
            uInfo.bind(7, def->dev_url);
            uInfo.bind(8, x.icon.to);
            uInfo.bind(9, c);
            uInfo.exec();
            
            
            // Aliases
            x.aliases.analyze();
            for(auto& s : x.aliases.added)
                iDefAlias.exec(x.id, s);
                
            // Prefixes
            x.prefixes.analyze();
            for(auto& s : x.prefixes.added)
                iDefPrefix.exec(x.id, s);

            // Reverses
            x.reverses.analyze();
            for(Class c : x.reverses.added)
                iDefReverse.exec(x.id, c);

            // Sources
            x.sources.analyze();
            for(Class c : x.sources.added)
                iDefSource.exec(x.id, c);

            // Suffixes
            x.suffixes.analyze();
            for(auto& s : x.suffixes.added)
                iDefSuffix.exec(x.id, s);

            // Tags
            x.tags.analyze();
            for(Tag t : x.tags.added)
                iTag.exec(x.id, t.id);
                
            // Targets
            x.targets.analyze();
            for(Class c : x.targets.added)
                iDefTarget.exec(x.id, c);

            // Uses
            x.uses.analyze();
            for(Class u : x.uses.added)
                iDefUse.exec(x.id, u.id);
            
                
                
            x.bases.to      = enumerate_hops<Class,hop_t>(c, cdb::def_use_set);
            x.bases.analyze();
            
            for(auto& j : x.bases.added){
                iBase.bind(1, x.id);
                iBase.bind(2, j.first.id);
                iBase.bind(3, j.second.cnt);
                iBase.exec();
            }
            for(auto& j : x.bases.modified){
                uBase.bind(1, j.second.cnt);
                uBase.bind(2, x.id);
                uBase.bind(3, j.first.id);
                uBase.exec();
            }
            
            
            x.deriveds.to   = enumerate_hops<Class,hop_t>(c, cdb::def_rev_use_set);
            x.deriveds.analyze();

            for(auto& j : x.deriveds.added){
                iBase.bind(1, j.first.id);
                iBase.bind(2, x.id);
                iBase.bind(3, j.second.cnt);
            }
            for(auto& j : x.deriveds.modified){
                uBase.bind(1, j.second.cnt);
                uBase.bind(2, j.first.id);
                uBase.bind(3, x.id);
            }
        }

        if(chg == Change::Removed){
            x.bases.removed         = x.bases.from;
            x.deriveds.removed      = x.deriveds.from;
            x.tags.removed          = x.tags.from;
            
            x.aliases.removed       = x.aliases.from;
            x.prefixes.removed      = x.prefixes.from;
            x.reverses.removed      = x.reverses.from;
            x.sources.removed       = x.sources.from;
            x.suffixes.removed      = x.suffixes.from;
            x.targets.removed       = x.targets.from;
            x.uses.removed          = x.uses.from;
        }
        
        if(chg != Change::Removed){
            for(Tag t : x.tags.removed)
                dTag.exec(x.id, t.id);
            for(auto& j : x.bases.removed)
                dBase.exec(x.id, j.first.id);
            for(auto& j : x.deriveds.removed)
                dBase.bind(j.first.id, x.id);
            
            for(auto& s : x.aliases.removed)
                dDefAlias.exec(x.id, s);
            for(auto& s : x.prefixes.removed)
                dDefPrefix.exec(x.id, s);
            for(Class u : x.reverses.removed)
                dDefReverse.exec(x.id, u.id);
            for(Class u : x.sources.removed)
                dDefSource.exec(x.id, u.id);
            for(auto& s : x.suffixes.removed)
                dDefSuffix.exec(x.id, s);
            for(Class u : x.targets.removed)
                dDefTarget.exec(x.id, u.id);
            for(Class u : x.uses.removed)
                dDefUse.exec(x.id, u.id);
        }
        
        
        Class::Notify::notify(x);

        if(chg == Change::Removed){
            static thread_local CacheQuery stmts[] = {
                CacheQuery("DELETE FROM " TBL_CLASS_ALIAS " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLSDEF_PREFIX " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLSDEF_SUFFIX " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_DEPEND " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_DEPEND " WHERE base=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_TAG " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLSDEF_USE " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_LOOKUP " WHERE class=? AND priority=1"),
                CacheQuery("DELETE FROM " TBL_CLASS_FIELD " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLASSES " WHERE id=?")
            };
            for(auto& sq : stmts)
                sq.exec(x.id);
        }
        
    }
    
    
        //  " TBL_CLASSES " go differently....
    
    void    s3_class(Document doc)
    {
        u_class(cdb::db_class(doc), Change::Added);
    }


    void    s3_class_propagate(Document doc)
    {
        Class       c   = cdb::class_(doc);
        if(!c)
            return ;
            
        std::string k   = cdb::key(c);
            
        //  
        //  Correct the inheritance
        //
            auto bases      = enumerate_hops<Class,hop_t>(c, cdb::def_use_set);
            auto derives    = enumerate_hops<Class,hop_t>(c, cdb::def_rev_use_set);
            
            static thread_local CacheQuery iBase("INSERT OR REPLACE INTO " TBL_CLASS_DEPEND " (class, base, hops) VALUES (?,?,?)");        
            
            for(auto& j : bases){
                iBase.bind(1, c.id);
                iBase.bind(2, j.first.id);
                iBase.bind(3, j.second.cnt);
                iBase.exec();
            }
            
            for(auto& j : derives){
                iBase.bind(1, j.first.id);
                iBase.bind(2, c.id);
                iBase.bind(3, j.second.cnt);
            }

    
        //
        //  Fields
        //
        
            static thread_local CacheQuery  sField("SELECT id FROM " TBL_FIELDS " WHERE class=?");
            
            FieldSet        fields;
            fields  = sField.set<Field>(0ULL);
            fields += sField.set<Field>(c.id);
            for(auto& j : bases)
                fields += sField.set<Field>(j.first.id);

            static thread_local CacheQuery  iField("INSERT INTO " TBL_CLASS_FIELD " (class,field) VALUES (?,?)");
            for(Field f : fields)
                iField.exec(c.id, f.id);
                
        //
        //  Sources
        //
        
            ClassSet    sources = cdb::def_source_set(c);
            for(auto& j : bases)
                sources += cdb::def_source_set(j.first);
            sources.erase(c);
                
            static thread_local CacheQuery  iSource("INSERT INTO " TBL_CLASS_SOURCE " (class,source) VALUES (?,?)");
            for(Class x : sources)
                iSource.exec(c.id, x.id);
                
        //
        //  Targets
        //
        
            ClassSet    targets = cdb::def_target_set(c);
            for(auto& j : bases)
                targets += cdb::def_target_set(j.first);
            targets.erase(c);
            static thread_local CacheQuery  iTarget("INSERT INTO " TBL_CLASS_TARGET " (class,target) VALUES (?,?)");
            for(Class x : targets)
                iTarget.exec(c.id, x.id);
            
        //
        //  Reverses
        //
        
            ClassSet    reverses = cdb::def_reverse_set(c);
            for(auto& j : bases)
                reverses += cdb::def_reverse_set(j.first);
            reverses.erase(c);
            static thread_local CacheQuery  iReverse("INSERT INTO " TBL_CLASS_REVERSE " (class,reverse) VALUES (?,?)");
            for(Class x : reverses)
                iReverse.exec(c.id, x.id);
            
        //
        //  LOOKUPS
        //
            
            string_set_t  aliases   = cdb::def_alias_set(c);
            for(auto& j : bases)
                aliases += cdb::def_alias_set(j.first);
            aliases.erase(k);
            
            string_set_t  prefixes   = cdb::def_prefix_set(c);
            for(auto& j : bases)
                prefixes += cdb::def_prefix_set(j.first);

            string_set_t  suffixes   = cdb::def_suffix_set(c);
            for(auto& j : bases)
                suffixes += cdb::def_suffix_set(j.first);
                
            string_set_t    lookups;
            
            for(auto& a : aliases)
                lookups.insert(capitalize(a));

            for(auto& p : prefixes)
                lookups.insert(capitalize(p+k));

            for(auto& p : prefixes)
                for(auto& s : suffixes)
                    lookups.insert(capitalize(p+k+s));
            
            for(auto& s : suffixes)
                lookups.insert(capitalize(k+s));

            for(auto& aa : aliases)
                for(auto& p : prefixes)
                    lookups.insert(capitalize(p+aa));
                    
            for(auto& aa : aliases)
                for(auto& p : prefixes)
                    for(auto& s : suffixes)
                        lookups.insert(capitalize(p+aa+s));

            for(auto& aa : aliases)
                for(auto& s : suffixes)
                    lookups.insert(capitalize(aa+s));
            
            static thread_local CacheQuery iLookup("INSERT OR IGNORE INTO " TBL_CLASS_LOOKUP " (class,alias,priority) VALUES (?,?,1)");
            for(auto& s : lookups)
                iLookup.exec(c.id, s);
            
    }
    
    void    s4_class()
    {
        class_graph_redraw();
        //  sends out notifications?
    }

    void    s5_class(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Class x = cdb::class_(doc);
                if(x){
                    u_class(x, Change::Removed, true);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Class x       = cdb::db_class(doc, &created);
        if(created){
            u_class(x, Change::Added, true);
        } else
            u_class(x, Change::Modified, true);
    }
    
    void    s5_class_icons(Fragment frag, Change chg)
    {
        Class    x   = cdb::class_(cdb::document(frag), true);
        if(!x)
            return ;
        u_class(x, Change::Modified, true);
    }


    //void    class_notify(Fragment frag,Change chg)
    //{
        //UClass::notify(frag, chg);
        //class_graph_redraw();
    //}
}
