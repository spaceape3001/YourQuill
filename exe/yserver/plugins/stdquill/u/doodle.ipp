////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/doodle/Doodle.hpp>
#include <mithril/doodle/DoodleCDB.hpp>
//#include <mithril/doodle/DoodleDiff.hpp>
#include <mithril/doodle/DoodleInfo.hpp>
//#include <mithril/doodle/DoodleData.hpp>

namespace {
    void    u_doodle(Doodle t, Change chg)
    {
        if(!t)
            return ;
        
        #if 0
        Doodle::Diff   x { 
            .x = t, 
            .id = t.id,
            .chg = chg, 
            .key = cdb::key(t)
        };
        
        if(chg != Change::Added){
            Doodle::Info       ii  = cdb::info(t);
            x.icon.from     = ii.icon;
            x.name.from     = std::move(ii.name);
            x.brief.from    = std::move(ii.brief);
        }
        
        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery uDoodleInfo("UPDATE " TBL_DOODLES " SET name=?,icon=?,brief=? WHERE id=?");
        static thread_local CacheQuery xDoodleStmts[] = {
            CacheQuery( "DELETE FROM " TBL_DOODLES " WHERE id=?" )
        };

        Document doc    = cdb::document(t);
        if(chg != Change::Removed){
            auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Doodle " << x.key << " cannot be read.";
                return;
            }
            
            x.icon.to       = cdb::best_image(doc);
            x.name.to       = std::move(def->name);
            x.brief.to      = std::move(def->brief);

            if(x.icon)
                uDocIcon.exec(x.icon.to.id, doc.id);

            if(x){
                uDoodleInfo.bind(1, x.name.to);
                uDoodleInfo.bind(2, x.icon.to.id);
                uDoodleInfo.bind(3, x.brief.to);
                uDoodleInfo.bind(4, x.id);
                uDoodleInfo.exec();
            } 
        }
        
        Doodle::Notify::notify(x);
        
        if(chg == Change::Removed){
            for(auto& sq : xDoodleStmts)
                sq.exec(t.id);
        }
        
        #endif
    }

    void    s3_doodle(Document doc)
    {
        bool        created     = false;
        Doodle x       = cdb::db_doodle(doc, &created);
        u_doodle(x, Change::Added);
    }

    void    s5_doodle(Fragment f, Change chg)
    {
        Document    doc = cdb::document(f);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Doodle x = cdb::doodle(doc);
                if(x){
                    u_doodle(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Doodle x       = cdb::db_doodle(doc, &created);
        if(created){
            u_doodle(x, Change::Added);
        } else
            u_doodle(x, Change::Modified);
    }
    
    void    s5_doodle_icons(Fragment f, Change chg)
    {
        Doodle    x   = cdb::doodle(cdb::document(f), true);
        if(!x)
            return ;
        u_doodle(x, Change::Modified);
    }
}
