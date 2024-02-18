////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    u_leaf(Leaf l, Change chg)
    {
        Document doc = cdb::document(l);
    
        Leaf::Diff   x { 
            .x      = l, 
            .id     = l.id,
            .chg    = chg, 
            .key    = cdb::key(l),
            .doc    = doc,
            .atom   = cdb::db_atom(doc)
        };
        Attribute::Report       diffrep;
        
        if(chg != Change::Added){
            Leaf::Info      ii  = cdb::info(l);
            
            x.icon.from         = ii.icon;
            x.title.from        = ii.title;
            x.brief.from        = ii.brief;
            x.abbr.from         = ii.abbr;
            x.tags.from         = cdb::tags_set(l);
        }

        static thread_local CacheQuery uInfo("UPDATE Leafs SET title=?, abbr=?, brief=?, icon=?, atom=? WHERE id=?");
        static thread_local CacheQuery dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
        static thread_local CacheQuery iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
        
        if(chg != Change::Removed){
            auto def        = cdb::merged(l, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Leaf " << x.key << " cannot be read.";
                return;
            }
            
            x.icon.to           = cdb::best_image(doc);
            x.title.to          = def->title();
            x.abbr.to           = def->abbr();
            x.brief.to          = def->brief();
            
            if(x.title.to.empty())
                x.title.to      = def->attrs.value(kv::key({"nick", "name"}));
            if(x.title.to.empty()){
                uInfo.bind(1, x.key);
            } else 
                uInfo.bind(1, x.title.to);
                
            uInfo.bind(2, x.abbr.to);
            uInfo.bind(3, x.brief.to);
            uInfo.bind(4, x.icon.to);
            uInfo.bind(5, x.atom.id);
            uInfo.bind(6, x.id);
            uInfo.exec();

            x.tags.to           = cdb::db_tags_set(def->tags());
            x.tags.analyze();
            
            for(Tag t : x.tags.added)
                iTag.exec(x.id, t.id);
                
            if(chg == Change::Added){
                diffrep = diff::additions(doc, def->attrs);
            } else {
                diffrep = diff::changes(doc, def->attrs);
            }
        } 
        
        if(chg == Change::Removed){
            diffrep = diff::changes(doc, {});
            x.tags.removed  = x.tags.from;
        }

        diffrep.exec_inserts();
        diffrep.exec_reindex();
        
        u_atom(x.atom, diffrep.items, chg, x.icon.to);

        //  Get the atom too
        Leaf::Notify::notify(x);
        
        diffrep.exec_removals();
        
        if(chg != Change::Removed){
            for(Tag t : x.tags.removed)
                dTag.exec(x.id, t.id);
        } else {
            static thread_local CacheQuery stmts[] = {
                CacheQuery( "DELETE FROM LTags WHERE leaf=?" ),
                CacheQuery( "DELETE FROM Leafs WHERE id=?" )
            };
            for(auto& sq : stmts)
                sq.exec(x.id);
        }        
    }
    
    void    s3_leaf(Document doc)
    {
        bool        created     = false;
        Leaf x       = cdb::db_leaf(doc, &created);
        u_leaf(x, Change::Added);
    }
    
    void    s5_leaf(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Leaf x = cdb::leaf(doc);
                if(x){
                    u_leaf(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Leaf x       = cdb::db_leaf(doc, &created);
        if(created){
            u_leaf(x, Change::Added);
        } else
            u_leaf(x, Change::Modified);
    }

    void    s5_leaf_icons(Fragment frag, Change chg)
    {
        Leaf    x   = cdb::leaf(cdb::document(frag), true);
        if(!x)
            return ;
        u_leaf(x, Change::Modified);
    }
}
