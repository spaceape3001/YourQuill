////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/style/Style.hpp>
#include <mithril/style/StyleCDB.hpp>
#include <mithril/style/StyleDiff.hpp>
#include <mithril/style/StyleInfo.hpp>
#include <mithril/style/StyleData.hpp>

namespace {
    void    u_style(Style t, Change chg)
    {
        if(!t)
            return ;
            
        Style::Diff   x { 
            .x = t, 
            .id = t.id,
            .chg = chg, 
            .key = cdb::key(t)
        };
        
        if(chg != Change::Added){
            Style::Info       ii  = cdb::info(t);
            x.icon.from     = ii.icon;
            x.name.from     = std::move(ii.name);
            x.brief.from    = std::move(ii.brief);
        }
        
        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery uStyleInfo("UPDATE " TBL_STYLES " SET name=?,icon=?,brief=? WHERE id=?");
        static thread_local CacheQuery xStyleStmts[] = {
            CacheQuery( "DELETE FROM " TBL_STYLES " WHERE id=?" )
        };

        Document doc    = cdb::document(t);
        if(chg != Change::Removed){
            auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Style " << x.key << " cannot be read.";
                return;
            }
            
            x.icon.to       = cdb::best_image(doc);
            x.name.to       = std::move(def->name);
            x.brief.to      = std::move(def->brief);

            if(x.icon)
                uDocIcon.exec(x.icon.to.id, doc.id);

            if(x){
                uStyleInfo.bind(1, x.name.to);
                uStyleInfo.bind(2, x.icon.to.id);
                uStyleInfo.bind(3, x.brief.to);
                uStyleInfo.bind(4, x.id);
                uStyleInfo.exec();
            } 
        }
        
        Style::Notify::notify(x);
        
        if(chg == Change::Removed){
            for(auto& sq : xStyleStmts)
                sq.exec(t.id);
        }
    }

    void    s3_style(Document doc)
    {
        bool        created     = false;
        Style x       = cdb::db_style(doc, &created);
        u_style(x, Change::Added);
    }

    void    s5_style(Fragment f, Change chg)
    {
        Document    doc = cdb::document(f);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Style x = cdb::style(doc);
                if(x){
                    u_style(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Style x       = cdb::db_style(doc, &created);
        if(created){
            u_style(x, Change::Added);
        } else
            u_style(x, Change::Modified);
    }
    
    void    s5_style_icons(Fragment f, Change chg)
    {
        Style    x   = cdb::style(cdb::document(f), true);
        if(!x)
            return ;
        u_style(x, Change::Modified);
    }
}
