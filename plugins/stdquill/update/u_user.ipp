////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    update_user(Document doc)
    {
        User        x   = db_user(doc);
        if(!x)
            return ;

        User::SharedData data = update_info(x);
        if(!data){
            yWarning() << "Unable to update user '" << key(x) << "' due to lack of data";
            return ;
        }
    }
    
    void    change_user(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(user(doc));
                return ;
            }
        }

        update_user(doc);
    }
 
    void    change_user_image(Fragment frag, Change)
    {
        update_icon(user(document(frag), true));
    }

    
    YQ_INVOKE( 
        u_change<change_user>(users_folder(), "*.user"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_user_image>(users_folder(), z);
    )
}
