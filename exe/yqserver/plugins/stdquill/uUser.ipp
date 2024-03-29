////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/DocumentCDB.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/user/UserData.hpp>
#include <mithril/wksp/CacheQuery.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    //! Updates user data from files, return merged data
    User::SharedData        u_user_update(User, cdb_options_t opts=0);


    void    u_user_erase(User u)
    {
        static thread_local CacheQuery d("DELETE FROM Users WHERE id=?");
        d.exec(u.id);
    }

    void    u_user_erase(Document doc)
    {
        u_user_erase(cdb::user(doc));
    }

    void    u_user_icon(User x)
    {
        if(!x)
            return ;
            
        Document    doc     = cdb::document(x);
        Image       img     = cdb::best_image(doc);
        static thread_local CacheQuery u1("UPDATE Users SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");
        
        if(cdb::icon(x) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }


    void    u_user_notify(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                u_user_erase(doc);
                return ;
            }
        }

        bool    created = false;
        User    x   = cdb::db_user(doc, &created);
        if(!x)
            return ;
            
        cdb_options_t   opts = cdb::IS_UPDATE|cdb::U_INFO;
        if(created)
            opts |= cdb::U_ICON;

        User::SharedData data = u_user_update(x, opts);
        if(!data){
            yWarning() << "Unable to update user '" << cdb::key(x) << "'";
            return ;
        }
    }

    void    u_user_notify_icons(Fragment frag, Change)
    {
        u_user_icon(cdb::user(cdb::document(frag), true));
    }

    void    u_user_stage3(Document doc)
    {
        User    x   = cdb::db_user(doc);
        User::SharedData data = u_user_update(x, cdb::DONT_LOCK|cdb::U_INFO|cdb::U_ICON);
        if(!data){
            yWarning() << "Unable to initialize user '" << cdb::key(x) << "'";
            return ;
        }
    }

    //! Updates user data from files, return merged data
    User::SharedData    u_user_update(User x, cdb_options_t opts)
    {
        if(!x)
            return User::SharedData();

        if(opts & cdb::U_ICON)
            u_user_icon(x);

        auto data  = cdb::merged(x, opts|cdb::IS_UPDATE);
        if(!data)
            return User::SharedData();

        static thread_local CacheQuery uInfo("UPDATE Users SET name=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=? WHERE id=?");
        if(opts & cdb::U_INFO){
            uInfo.bind(1, data->name);
            uInfo.bind(2, data->brief);
            uInfo.bind(3, data->permissions.is_set( Permission::Owner ));
            uInfo.bind(4, data->permissions.is_set( Permission::Admin ));
            uInfo.bind(5, data->permissions.is_set( Permission::Writer ));
            uInfo.bind(6, data->permissions.is_set( Permission::Reader ));
            uInfo.bind(7, x.id);
            uInfo.exec();
        }

        return data;
    }
}
