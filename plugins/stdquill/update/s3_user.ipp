////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    s3_user(Document doc)
    {
        User    x       = db_user(doc);
        Image   img     = best_image(doc);
        
        User::SharedData data = merged(x, IS_UPDATE | DONT_LOCK);
        if(!data){
            yWarning() << "Unable to update tag '" << key(x) << "' due to lack of data";
            return ;
        }
        
        static thread_local SQ u("UPDATE Users SET name=?,brief=?,icon=?,is_owner=?,is_admin=?,is_writer=?,is_reader=? WHERE id=?");
        auto u_af = u.af();
        u.bind(1, data->name);
        u.bind(2, data->brief);
        u.bind(3, img.id);
        u.bind(4, data->permissions.is_set( Permission::Owner ));
        u.bind(5, data->permissions.is_set( Permission::Admin ));
        u.bind(6, data->permissions.is_set( Permission::Writer ));
        u.bind(7, data->permissions.is_set( Permission::Reader ));
        u.bind(8, x.id);
        u.exec();
    }
    
    YQ_INVOKE( u_stage3<s3_user>(users_folder(), "*.user"); )
}
