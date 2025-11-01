////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/novel/Novel.hpp>
#include <yq/mithril/novel/NovelCDB.hpp>
#include <yq/mithril/novel/NovelDiff.hpp>


namespace {
    void    u_novel(Novel v, Change chg)
    {
        if(!v)
            return ;

        Novel::Diff x {
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


        static thread_local CacheQuery uInfo("UPDATE " TBL_NOVELS " SET title=?,icon=? WHERE id=?");
        static thread_local CacheQuery dTag("DELETE FROM " TBL_NOVEL_TAG " WHERE novel=? AND tag=?");
        static thread_local CacheQuery iTag("INSERT INTO " TBL_NOVEL_TAG " (novel, tag) VALUES (?,?)");
        static thread_local CacheQuery xNovel[] = {
            CacheQuery( "DELETE FROM " TBL_NOVEL_TAG " WHERE novel=?" ),
            CacheQuery( "DELETE FROM " TBL_NOVELS " WHERE id=?" )
        };

        if(chg != Change::Removed){
            #if 0
                auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
                if(!def){
                    yInfo() << "Novel " << x.key << " cannot be read.";
                    return;
                }
            #endif

            //  Load the novel here....
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

        Novel::Notify::notify(x);
        
        if(chg != Change::Removed){
            for(Tag t : x.tags.removed)
                dTag.exec(x.id, t.id);
        } else {
            for(auto& sq : xNovel)
                sq.exec(v.id);
        }
    }
    
    void    s3_novel(Document doc)
    {
        for(Fragment frag : cdb::fragments(doc))
            u_novel(cdb::db_novel(frag), Change::Added);
    }
    
    void    s5_novel(Fragment frag, Change chg)
    {
        Novel  v;
        switch(chg){
        case Change::Added:
        case Change::Modified:
            u_novel(cdb::db_novel(frag), chg);
            break;
        case Change::Removed:
            v   = cdb::novel(frag);
            if(v)
                u_novel(v, chg);
            break;
        default:
            break;
        }
    }

    void    s5_novel_icons(Fragment frag, Change chg)
    {
        Novel    x   = cdb::novel(frag, true);
        if(!x)
            return ;
        u_novel(x, Change::Modified);
    }
}
