////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    update_user(User x)
    {
        User::SharedData data = merged(x, IS_UPDATE);
        if(!data){
            yWarning() << "Unable to update tag '" << key(x) << "' due to lack of data";
            return ;
        }
        
        static thread_local SQ u("UPDATE Users SET name=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=? WHERE id=?");
        auto u_af = u.af();
        u.bind(1, data->name);
        u.bind(2, data->brief);
        u.bind(3, data->permissions.is_set( Permission::Owner ));
        u.bind(4, data->permissions.is_set( Permission::Admin ));
        u.bind(5, data->permissions.is_set( Permission::Writer ));
        u.bind(6, data->permissions.is_set( Permission::Reader ));
        u.bind(7, x.id);
        u.exec();
    }
    
    void    remove_user(User x)
    {
        static thread_local SQ u("DELETE FROM Users WHERE id=?");
        u.exec(x.id);
    }
    
    void    change_user(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        User        x   = db_user(doc);
        if(!x)
            return ;

        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                remove_user(x);
                return ;
            }
        }

        update_user(x);
    }
 
    void    change_user_image(Fragment frag, Change)
    {
        Document    doc = document(frag);
        std::string bk  = base_key(doc);
        if(bk.empty())
            return;
        bk += ".user";
        doc             = child_document(users_folder(), bk);
        if(!doc)
            return ;
            
        User     x   = user(doc);
        if(x){
            Image   img0    = icon(x);
            Image   img1    = best_image(doc);
            if(img0 == img1)
                return ;
            
            static thread_local SQ u1("UPDATE Users SET icon=? WHERE id=?");
            auto af = u1.af();
            u1.bind(1, img1.id);
            u1.bind(2, x.id);
            u1.exec();
        }
    }

    
    YQ_INVOKE( 
        u_change<change_user>(users_folder(), "*.user"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_user_image>(users_folder(), z);
    )
}
