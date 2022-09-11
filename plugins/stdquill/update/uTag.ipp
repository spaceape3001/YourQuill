////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uTag.hpp"

namespace yq {
    Tag::SharedData         update(Tag x, cdb_options_t opts)
    {
        if(!x)
            return Tag::SharedData();

        if(opts & U_ICON)
            update_icon(x);

        auto data  = merged(x, opts);
        if(!data)
            return Tag::SharedData();
         
        static thread_local SQ uInfo("UPDATE Tags SET name=?,brief=? WHERE id=?");
        static thread_local SQ uLeaf("UPDATE Tags SET leaf=? WHERE id=?");
        
        if(opts&U_INFO){
            uInfo.bind(1, data->name);
            uInfo.bind(2, data->brief);
            uInfo.bind(3, x.id);
            uInfo.exec();
        }
        if(opts&U_LEAF){
            Leaf    l   = leaf(data->leaf);
            uLeaf.exec(l.id, x.id);
        }
        
        return data;
    }

    void                    update_icon(Tag x)
    {
        if(!x)
            return ;

        Document    doc     = document(x);
        Image       img     = best_image(doc);

        static thread_local SQ u1("UPDATE Tags SET icon=? WHERE id=?");
        static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");
        
        if(icon(x) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }

}
