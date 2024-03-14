////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    u_category(Category t, Change chg)
    {
        if(!t)
            return ;
            
        Category::Diff   x { 
            .x = t, 
            .id = t.id,
            .chg = chg, 
            .key = cdb::key(t)
        };
        
        if(chg != Change::Added){
            Category::Info ii  = cdb::info(t);
            x.icon.from     = ii.icon;
            x.name.from     = std::move(ii.name);
            x.brief.from    = std::move(ii.brief);
        }

        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery uCategoryInfo("UPDATE Categories SET name=?,icon=?,brief=? WHERE id=?");
        
        Document doc    = cdb::document(t);
        if(chg != Change::Removed){
            auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Category " << x.key << " cannot be read.";
                return;
            }
            
            x.icon.to       = cdb::best_image(doc);
            x.name.to       = std::move(def->name);
            x.brief.to      = std::move(def->brief);
            x.notes         = std::move(def->notes);
            
            if(x.icon){
                uDocIcon.exec(x.icon.to.id, doc.id);
            }

            uCategoryInfo.bind(1, x.name.to);
            uCategoryInfo.bind(2, x.icon.to.id);
            uCategoryInfo.bind(3, x.brief.to);
            uCategoryInfo.bind(4, x.id);
            uCategoryInfo.exec();
        }

        static thread_local CacheQuery xCategoryStmts[] = {
            CacheQuery( "UPDATE Classes SET category=0 WHERE category=?" ),
            CacheQuery( "DELETE FROM Categories WHERE id=?" )
        };
        
        Category::Notify::notify(x);
        
        if(chg == Change::Removed){
            for(auto& sq : xCategoryStmts)
                sq.exec(x.id);
        }
    }

    void    s3_category(Document doc)
    {
        bool        created     = false;
        Category x       = cdb::db_category(doc, &created);
        u_category(x, Change::Added);
    }
    

    void    s5_category(Fragment f, Change chg)
    {
        Document    doc = cdb::document(f);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Category x = cdb::category(doc);
                if(x){
                    u_category(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Category x       = cdb::db_category(doc, &created);
        if(created){
            u_category(x, Change::Added);
        } else
            u_category(x, Change::Modified);
    }
    
    void    s5_category_icons(Fragment f, Change chg)
    {
        Category    x   = cdb::category(cdb::document(f), true);
        if(!x)
            return ;
        u_category(x, Change::Modified);
    }
}
