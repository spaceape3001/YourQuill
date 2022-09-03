////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uLeaf.hpp"
#include <basic/Vector.hpp>
#include <kernel/db/SQ.hpp>

namespace yq {
    ULeaf&  uget(Leaf a)
    {
        static Vector<ULeaf*>   s_data;
        s_data.resize_if_under(a.id+1, 4096, nullptr);
        ULeaf*& p   = s_data[a.id];
        if(!p)
            p       = new ULeaf(a);
        return *p;
    }


    Leaf::SharedData         update(Leaf x, cdb_options_t opts)
    {
        if(!x)
            return Leaf::SharedData();
        
        if(opts & U_ICON)
            update_icon(x);
        
        auto data  = merged(x, opts|IS_UPDATE);
        if(!data)
            return Leaf::SharedData();

        static thread_local SQ dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
        static thread_local SQ iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
        static thread_local SQ uInfo("UPDATE Leafs SET title=?, abbr=?, brief=? WHERE id=?");

        if(opts & U_INFO){
            std::string_view title   = data->title();
            std::string_view abbr    = data->abbr();
            std::string_view brief   = data->brief();
            if(title.empty())
                title       = data->attrs.value(kv::key({"nick", "name"}));
            
            if(title.empty()){
                uInfo.bind(1, key(x));  // fall back (for now) 
                                    // TODO ... make this smarter (later)
            } else 
                uInfo.bind(1, title);
            uInfo.bind(2, abbr);
            uInfo.bind(3, brief);
            uInfo.bind(4, x.id);
            uInfo.exec();
        }
        
        if(opts & U_TAGS){
            std::set<Tag>   old_tags = tags_set(x);
            std::set<Tag>   new_tags = tags_set(data->tags(), true);

            auto ch_tag = add_remove(old_tags, new_tags);
            iTag.batch(x.id, ids_for(ch_tag.added));
            dTag.batch(x.id, ids_for(ch_tag.removed));
            
        }
        
        return data;
    }

    void                update_icon(Leaf x)
    {
        if(!x)
            return ;
            
        Document    doc     = document(x);
        Image       img     = best_image(doc);
        static thread_local SQ u1("UPDATE Leafs SET icon=? WHERE id=?");
        static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

        if(icon(x) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }
}

