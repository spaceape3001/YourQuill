////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uUser.hpp"

namespace yq {

    User::SharedData         update(User x, cdb_options_t opts)
    {
        if(!x)
            return User::SharedData();

        if(opts & U_ICON)
            update_icon(x);
    
        auto data  = merged(x, opts|IS_UPDATE);
        if(!data)
            return User::SharedData();

        static thread_local SQ uInfo("UPDATE Users SET name=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=? WHERE id=?");
        if(opts & U_INFO){
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
    
    void                update_icon(User x)
    {
        if(!x)
            return ;
            
        Document    doc     = document(x);
        Image       img     = best_image(doc);
        static thread_local SQ u1("UPDATE Users SET icon=? WHERE id=?");
        static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");
        
        if(icon(x) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }
}
