////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    s3_category(Document doc)
    {
        Category    x       = db_category(doc);
        
        Category::SharedData data = update_info(x);
        if(!data){
            yWarning() << "Unable to update category '" << key(x) << "' due to lack of data";
            return ;
        }
        
        update_icon(x);
    }
    
    YQ_INVOKE( u_stage3<s3_category>(categories_folder(), "*.cat"); )
}
