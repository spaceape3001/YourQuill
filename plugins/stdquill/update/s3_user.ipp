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
        
        User::SharedData data = update_info(x, DONT_LOCK);
        if(!data){
            yWarning() << "Unable to update tag '" << key(x) << "' due to lack of data";
            return ;
        }
        
        update_icon(x);
    }
    
    YQ_INVOKE( u_stage3<s3_user>(users_folder(), "*.user"); )
}
