////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uCategory.hpp"

namespace yq {
    void                update_icon(Category x)
    {
        if(!x)
            return ;

        static thread_local SQ u1("UPDATE Categories SET icon=? WHERE id=?");
        static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

        Document    doc     = document(x);
        Image       img     = best_image(doc);
        
        if(img != icon(x)){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }
    
    Category::SharedData           update_this(Category x, cdb_options_t opts)
    {
        if(!x)
            return Category::SharedData();

        if(opts & U_ICON)
            update_icon(x);
        
        Category::SharedData data = merged(x, opts);
        if(!data)
            return Category::SharedData();
            
        static thread_local SQ uInfo("UPDATE Categories SET name=?,brief=? WHERE id=?");
        
        if(opts & U_INFO){
            uInfo.bind(1, data->name);
            uInfo.bind(2, data->brief);
            uInfo.bind(3, x.id);
            uInfo.exec();
        }
        return data;
    }
}
