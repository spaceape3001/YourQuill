////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uUser.hpp"

#include <basic/Logging.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/user/UserCDB.hpp>

namespace yq {
    namespace update {
        void                    user_erase(User u)
        {
            static thread_local cdb::SQ d("DELETE FROM Users WHERE id=?");
            d.exec(u.id);
        }

        void                    user_erase(Document doc)
        {
            user_erase(cdb::user(doc));
        }

        void                    user_icon(User x)
        {
            if(!x)
                return ;
                
            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            static thread_local cdb::SQ u1("UPDATE Users SET icon=? WHERE id=?");
            static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");
            
            if(cdb::icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }
        
        
        void                    user_notify(Fragment frag, Change chg)
        {
            Document    doc = cdb::document(frag);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    user_erase(doc);
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

            User::SharedData data = user_update(x, opts);
            if(!data){
                yWarning() << "Unable to update user '" << cdb::key(x) << "'";
                return ;
            }
        }
        
        void                    user_notify_icons(Fragment frag, Change)
        {
            user_icon(cdb::user(cdb::document(frag), true));
        }
        
        void                    user_stage3(Document doc)
        {
            User    x   = cdb::db_user(doc);
            User::SharedData data = user_update(x, cdb::DONT_LOCK|cdb::U_INFO|cdb::U_ICON);
            if(!data){
                yWarning() << "Unable to initialize user '" << cdb::key(x) << "'";
                return ;
            }
        }
        
        //! Updates user data from files, return merged data
        User::SharedData        user_update(User x, cdb_options_t opts)
        {
            if(!x)
                return User::SharedData();

            if(opts & cdb::U_ICON)
                user_icon(x);
        
            auto data  = cdb::merged(x, opts|cdb::IS_UPDATE);
            if(!data)
                return User::SharedData();

            static thread_local cdb::SQ uInfo("UPDATE Users SET name=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=? WHERE id=?");
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
}
