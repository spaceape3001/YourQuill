////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
    

    void    u_class(Class c, Change chg)
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
            Class::Info     ii  = cdb::info(c);
            x.category.from     = ii.category;
            x.icon.from         = ii.icon;
            x.name.from         = std::move(ii.name);
            x.plural.from       = std::move(ii.plural);
            x.brief.from        = std::move(ii.brief);
            x.tags.from         = cdb::tag_set(c);
            x.uses.from         = cdb::use_set(c);
            x.bases.from        = cdb::base_hops(c);
            x.deriveds.from     = cdb::derived_hops(c);
        }
        
        static thread_local CacheQuery uInfo("UPDATE " TBL_CLASSES " SET name=?, plural=?, brief=?, category=?, binding=?, url=?, devurl=?, icon=? WHERE id=?");
        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery iTag("INSERT INTO " TBL_CLASS_TAG " (class, tag) VALUES (?,?)");
        static thread_local CacheQuery dTag("DELETE FROM " TBL_CLASS_TAG " WHERE class=? AND tag=?");
        static thread_local CacheQuery iUse("INSERT INTO " TBL_CLASS_USE " (class, use) VALUES (?,?)");
        static thread_local CacheQuery dUse("DELETE FROM " TBL_CLASS_USE " WHERE class=? AND use=?");
        static thread_local CacheQuery iBase("INSERT OR REPLACE INTO " TBL_CLASS_DEPEND " (class, base, hops) VALUES (?,?,?)");
        static thread_local CacheQuery dBase("DELETE FROM " TBL_CLASS_DEPEND " WHERE class=? AND base=?");
        static thread_local CacheQuery uBase("UPDATE " TBL_CLASS_DEPEND " SET hops=? WHERE class=? AND base=?");

        if(chg != Change::Removed){
            auto def        = cdb::merged(c, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Class " << x.key << " cannot be read.";
                return;
            }

            x.icon.to           = cdb::best_image(doc);
            x.category.to       = cdb::category(def->category);
            x.name.to           = std::move(def->name);
            if(x.name.to.empty())
                x.name.to       = x.key;
            x.plural.to         = std::move(def->plural);
            x.brief.to          = std::move(def->brief);
            x.tags.to           = make_set(cdb::db_tags(def->tags));
            x.uses.to           = make_set(cdb::db_classes(def->use));
            
            
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
            
            
            // Tags
            x.tags.analyze();
            
            for(Tag t : x.tags.added)
                iTag.exec(x.id, t.id);

            // Uses
            x.uses.analyze();
            for(Class u : x.uses.added)
                iUse.exec(x.id, u.id);
                
            x.bases.to      = enumerate_hops<Class,hop_t>(c, cdb::use_set);
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
            
            
            x.deriveds.to   = enumerate_hops<Class,hop_t>(c, cdb::rev_use_set);
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
            x.tags.removed      = x.tags.from;
            x.uses.removed      = x.uses.from;
            x.bases.removed     = x.bases.from;
            x.deriveds.removed  = x.deriveds.from;
        }
        
        if(chg != Change::Removed){
            for(Tag t : x.tags.removed)
                dTag.exec(x.id, t.id);
            for(Class u : x.uses.removed)
                dUse.exec(x.id, u.id);
            for(auto& j : x.bases.removed){
                dBase.bind(1, x.id);
                dBase.bind(2, j.first.id);
                dBase.exec();
            }
            for(auto& j : x.deriveds.removed){
                dBase.bind(1, j.first.id);
                dBase.bind(2, x.id);
                dBase.exec();
            }
        }
        
        
        Class::Notify::notify(x);

        if(chg == Change::Removed){
            static thread_local CacheQuery stmts[] = {
                CacheQuery("DELETE FROM " TBL_CLASS_ALIAS " WHERE class=?"),
                CacheQuery("DELETE FROM CPrefix WHERE class=?"),
                CacheQuery("DELETE FROM CSuffix WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_DEPEND " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_DEPEND " WHERE base=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_TAG " WHERE class=?"),
                CacheQuery("DELETE FROM " TBL_CLASS_USE " WHERE class=?"),
                CacheQuery("DELETE FROM CLookup WHERE class=? AND priority=1"),
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


    void    s3_class_deduce(Document doc)
    {
    }

    void    s3_class_propagate(Document doc)
    {
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
                    u_class(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Class x       = cdb::db_class(doc, &created);
        if(created){
            u_class(x, Change::Added);
        } else
            u_class(x, Change::Modified);
    }
    
    void    s5_class_icons(Fragment frag, Change chg)
    {
        Class    x   = cdb::class_(cdb::document(frag), true);
        if(!x)
            return ;
        u_class(x, Change::Modified);
    }


    //void    class_notify(Fragment frag,Change chg)
    //{
        //UClass::notify(frag, chg);
        //class_graph_redraw();
    //}
}
