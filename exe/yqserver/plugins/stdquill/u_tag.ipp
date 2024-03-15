////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    u_tag(Tag t, Change chg)
    {
        if(!t)
            return ;
            
        Tag::Diff   x { 
            .x = t, 
            .id = t.id,
            .chg = chg, 
            .key = cdb::key(t)
        };
        
        if(chg != Change::Added){
            Tag::Info       ii  = cdb::info(t);
            x.leaf.from     = ii.leaf;
            x.icon.from     = ii.icon;
            x.name.from     = std::move(ii.name);
            x.brief.from    = std::move(ii.brief);
        }
        
        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery uTagInfo("UPDATE " TBL_TAGS " SET name=?,icon=?,leaf=?,brief=? WHERE id=?");
        static thread_local CacheQuery xTagStmts[] = {
            CacheQuery( "DELETE FROM " TBL_CLASS_TAG " WHERE tag=?" ),
            CacheQuery( "DELETE FROM " TBL_FIELD_TAG " WHERE tag=?" ),
            CacheQuery( "DELETE FROM " TBL_LEAF_TAG " WHERE tag=?" ),
            CacheQuery( "DELETE FROM " TBL_TAGS " WHERE id=?" )
        };

        Document doc    = cdb::document(t);
        if(chg != Change::Removed){
            auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Tag " << x.key << " cannot be read.";
                return;
            }
            
            x.icon.to       = cdb::best_image(doc);
            x.name.to       = std::move(def->name);
            x.leaf.to       = cdb::leaf(def->leaf);
            x.brief.to      = std::move(def->brief);
            x.notes         = std::move(def->notes);

            if(x.icon)
                uDocIcon.exec(x.icon.to.id, doc.id);

            if(x){
                uTagInfo.bind(1, x.name.to);
                uTagInfo.bind(2, x.icon.to.id);
                uTagInfo.bind(3, x.leaf.to.id);
                uTagInfo.bind(4, x.brief.to);
                uTagInfo.bind(5, x.id);
                uTagInfo.exec();
            } 
        }
        
        Tag::Notify::notify(x);
        
        if(chg == Change::Removed){
            for(auto& sq : xTagStmts)
                sq.exec(t.id);
        }
    }

    void    s3_tag(Document doc)
    {
        bool        created     = false;
        Tag x       = cdb::db_tag(doc, &created);
        u_tag(x, Change::Added);
    }
    
    void    s3_tag_leaf(Document doc)
    {
        Tag    x   = cdb::tag(doc);
        if(!x)
            return ;
        u_tag(x, Change::Modified);
    }

    void    s5_tag(Fragment f, Change chg)
    {
        Document    doc = cdb::document(f);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Tag x = cdb::find_tag(doc);
                if(x){
                    u_tag(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Tag x       = cdb::db_tag(doc, &created);
        if(created){
            u_tag(x, Change::Added);
        } else
            u_tag(x, Change::Modified);
    }
    
    void    s5_tag_icons(Fragment f, Change chg)
    {
        Tag    x   = cdb::find_tag(cdb::document(f), true);
        if(!x)
            return ;
        u_tag(x, Change::Modified);
    }
}
