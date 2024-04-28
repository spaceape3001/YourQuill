////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/drafter/Drafter.hpp>
#include <mithril/drafter/DrafterCDB.hpp>
#include <mithril/drafter/DrafterDiff.hpp>
#include <mithril/drafter/DrafterInfo.hpp>
#include <mithril/drafter/DrafterData.hpp>

namespace {
    void    u_drafter(Drafter t, Change chg)
    {
        if(!t)
            return ;
            
        Drafter::Diff   x { 
            .x = t, 
            .id = t.id,
            .chg = chg, 
            .key = cdb::key(t)
        };
        
        if(chg != Change::Added){
            Drafter::Info       ii  = cdb::info(t);
            x.icon.from     = ii.icon;
            x.name.from     = std::move(ii.name);
            x.brief.from    = std::move(ii.brief);
        }
        
        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery uDrafterInfo("UPDATE " TBL_DRAFTERS " SET name=?,icon=?,brief=? WHERE id=?");
        static thread_local CacheQuery xDrafterStmts[] = {
            CacheQuery( "DELETE FROM " TBL_DRAFTERS " WHERE id=?" )
        };

        Document doc    = cdb::document(t);
        if(chg != Change::Removed){
            auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Drafter " << x.key << " cannot be read.";
                return;
            }
            
            x.icon.to       = cdb::best_image(doc);
            x.name.to       = std::move(def->name);
            x.brief.to      = std::move(def->brief);

            if(x.icon)
                uDocIcon.exec(x.icon.to.id, doc.id);

            if(x){
                uDrafterInfo.bind(1, x.name.to);
                uDrafterInfo.bind(2, x.icon.to.id);
                uDrafterInfo.bind(3, x.brief.to);
                uDrafterInfo.bind(4, x.id);
                uDrafterInfo.exec();
            } 
        }
        
        Drafter::Notify::notify(x);
        
        if(chg == Change::Removed){
            for(auto& sq : xDrafterStmts)
                sq.exec(t.id);
        }
    }

    void    s3_drafter(Document doc)
    {
        bool        created     = false;
        Drafter x       = cdb::db_drafter(doc, &created);
        u_drafter(x, Change::Added);
    }

    void    s5_drafter(Fragment f, Change chg)
    {
        Document    doc = cdb::document(f);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Drafter x = cdb::drafter(doc);
                if(x){
                    u_drafter(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Drafter x       = cdb::db_drafter(doc, &created);
        if(created){
            u_drafter(x, Change::Added);
        } else
            u_drafter(x, Change::Modified);
    }
    
    void    s5_drafter_icons(Fragment f, Change chg)
    {
        Drafter    x   = cdb::drafter(cdb::document(f), true);
        if(!x)
            return ;
        u_drafter(x, Change::Modified);
    }
}
