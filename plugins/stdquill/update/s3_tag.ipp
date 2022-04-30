////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    s3_tag(Document doc)
    {
        Tag     x       = db_tag(doc);
        
        Tag::SharedData data = update_info(x, DONT_LOCK);
        if(!data){
            yWarning() << "Unable to update tag '" << key(x) << "' due to lack of data";
            return ;
        }
        
        update_icon(x);
    }
    
    YQ_INVOKE( u_stage3<s3_tag>(tags_folder(), "*.tag"); )
}
