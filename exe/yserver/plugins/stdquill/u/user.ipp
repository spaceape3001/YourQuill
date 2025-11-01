////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/user/User.hpp>
#include <yq/mithril/user/UserCDB.hpp>
#include <yq/mithril/user/UserData.hpp>
#include <yq/mithril/user/UserDiff.hpp>
#include <yq/mithril/user/UserInfo.hpp>

namespace {
    void    u_user(User u, Change chg)
    {
        if(!u)
            return ;
            
        User::Diff   x { 
            .x = u, 
            .id = u.id,
            .chg = chg, 
            .key = cdb::key(u)
        };
        
        if(chg != Change::Added){
            User::Info       ii  = cdb::info(u);
            x.icon.from     = ii.icon;
            x.name.from     = std::move(ii.name);
            x.brief.from    = std::move(ii.brief);
            x.owner.from    = ii.is_owner;
            x.admin.from    = ii.is_admin;
            x.writer.from   = ii.is_writer;
            x.reader.from   = ii.is_reader;
            x.guest.from    = ii.is_guest;
        }
        
        static thread_local CacheQuery uDocIcon("UPDATE " TBL_DOCUMENTS " SET icon=? WHERE id=?");
        static thread_local CacheQuery uInfo("UPDATE " TBL_USERS " SET name=?,icon=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=?,is_guest=? WHERE id=?");

        Document    doc     = cdb::document(u);
        if(chg != Change::Removed){
            auto def        = cdb::merged(u, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "User " << x.key << " cannot be read.";
                return;
            }
            
            x.icon.to       = cdb::best_image(doc);
            x.name.to       = std::move(def->name);
            x.brief.to      = std::move(def->brief);
            x.owner.to      = def->permissions(Permission::Owner);
            x.admin.to      = def->permissions(Permission::Admin);
            x.writer.to     = def->permissions(Permission::Writer);
            x.reader.to     = def->permissions(Permission::Reader);
            x.guest.to      = def->permissions(Permission::Guest);
            
            if(x.icon)
                uDocIcon.exec(x.icon.to.id, doc.id);
            
            if(x){
                uInfo.bind(1, x.name.to);
                uInfo.bind(2, x.icon.to.id);
                uInfo.bind(3, x.brief.to);
                uInfo.bind(4, x.owner.to);
                uInfo.bind(5, x.admin.to);
                uInfo.bind(6, x.writer.to);
                uInfo.bind(7, x.reader.to);
                uInfo.bind(8, x.guest.to);
                uInfo.bind(9, x.id);
                uInfo.exec();
            }
        }
        
        User::Notify::notify(x);
        
        static thread_local CacheQuery xUserStmts[] = {
            CacheQuery( "DELETE FROM " TBL_USERS " WHERE id=?" )
        };

        if(chg == Change::Removed){
            for(auto& sq : xUserStmts)
                sq.exec(x.id);
        }
    }
    
    void    s3_user(Document doc)
    {
        bool        created     = false;
        User x       = cdb::db_user(doc, &created);
        u_user(x, Change::Added);
    }
    
    void    s5_user(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                User x = cdb::user(doc);
                if(x){
                    u_user(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        User x       = cdb::db_user(doc, &created);
        if(created){
            u_user(x, Change::Added);
        } else
            u_user(x, Change::Modified);
    }
    
    void    s5_user_icons(Fragment frag, Change chg)
    {
        User    x   = cdb::user(cdb::document(frag), true);
        if(!x)
            return ;
        u_user(x, Change::Modified);
    }
}
