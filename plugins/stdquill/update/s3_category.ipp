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
        Image       img     = best_image(doc);
        
        Category::SharedData data = merged(x, IS_UPDATE | DONT_LOCK);
        if(!data){
            yWarning() << "Unable to update category '" << key(x) << "' due to lack of data";
            return ;
        }
        
        static thread_local SQ u("UPDATE Categories SET name=?,brief=?,icon=? WHERE id=?");
        auto u_af = u.af();
        u.bind(1, data->name);
        u.bind(2, data->brief);
        u.bind(3, img.id);
        u.bind(4, x.id);
        u.exec();
    }
    
    YQ_INVOKE( u_stage3<s3_category>(categories_folder(), "*.cat"); )
}
