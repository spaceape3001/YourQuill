////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/doodle/Doodle.hpp>
#include <yq/mithril/doodle/DoodleCDB.hpp>
#include <yq/mithril/doodle/DoodleDiff.hpp>
#include <yq/mithril/doodle/DoodleInfo.hpp>

namespace {
    void    u_doodle(Doodle v, Change chg)
    {
        if(!v)
            return ;
            
        
        Doodle::Diff   x { 
            .x      = v, 
            .id     = v.id,
            .frag   = cdb::fragment(v),
            .doc    = cdb::document(v),
            .chg    = chg
        };
        
        if(chg != Change::Added){
            x.title.from    = cdb::title(v);
            x.tags.from     = cdb::tags_set(v);
        }
        
        static thread_local CacheQuery xDoodle[] = {
            CacheQuery( "DELETE FROM " TBL_DOODLES " WHERE id=?" ),
            CacheQuery( "DELETE FROM " TBL_DOODLE_TAG " WHERE doodle=?" )
        };
        static thread_local CacheQuery uInfo("UPDATE " TBL_DOODLES " SET title=?,icon=? WHERE id=?");
        static thread_local CacheQuery dTag("DELETE FROM " TBL_DOODLE_TAG " WHERE doodle=? AND tag=?");
        static thread_local CacheQuery iTag("INSERT INTO " TBL_DOODLE_TAG " (doodle, tag) VALUES (?,?)");

        if(chg != Change::Removed){
            #if 0
                auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
                if(!def){
                    yInfo() << "Doodle " << x.key << " cannot be read.";
                    return;
                }
            #endif

            //  Load the doodle here....
            //  Tags....
            
            uInfo.bind(1, x.title.to);
            uInfo.bind(2, x.icon.to);
            uInfo.bind(3, x.id);
            uInfo.exec();
            
            //x.tags.to = 
            x.tags.analyze();
            for(Tag t : x.tags.added)
                iTag.exec(x.id, t.id);
            
        }

        if(chg == Change::Removed){
            x.tags.removed  = x.tags.from;
        }
        
        Doodle::Notify::notify(x);
        
        if(chg != Change::Removed){
            for(Tag t : x.tags.removed)
                dTag.exec(x.id, t.id);
        } else {
            for(auto& sq : xDoodle)
                sq.exec(v.id);
        }
    }

    void    s3_doodle(Document doc)
    {
        for(Fragment frag : cdb::fragments(doc))
            u_doodle(cdb::db_doodle(frag), Change::Added);
    }

    void    s5_doodle(Fragment frag, Change chg)
    {
        Doodle  v;
        switch(chg){
        case Change::Added:
        case Change::Modified:
            u_doodle(cdb::db_doodle(frag), chg);
            break;
        case Change::Removed:
            v   = cdb::doodle(frag);
            if(v)
                u_doodle(v, chg);
            break;
        default:
            break;
        }
    }
    
    void    s5_doodle_icons(Fragment f, Change chg)
    {
        Doodle    x   = cdb::doodle(f, true);
        if(!x)
            return ;
        u_doodle(x, Change::Modified);
    }
}
