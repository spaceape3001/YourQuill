////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "u_tag.hpp"

#include <basic/Logging.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/leaf/LeafCDB.hpp>
#include <kernel/org/TagCDB.hpp>
#include <kernel/org/TagData.hpp>


namespace yq {
    void    u_tag_erase(Tag x)
    {
        using cdb::SQ;
        
        static thread_local SQ  stmts[] = {
            SQ( "DELETE FROM CTags WHERE tag=?" ),
            SQ( "DELETE FROM FTags WHERE tag=?" ),
            SQ( "DELETE FROM LTags WHERE tag=?" ),
            SQ( "DELETE FROM Tags WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
    
    void    u_tag_erase(Document doc)
    {
        u_tag_erase(cdb::tag(doc));
    }

    void    u_tag_icon(Tag x)
    {
        if(!x)
            return ;

        Document    doc     = cdb::document(x);
        Image       img     = cdb::best_image(doc);

        if(cdb::icon(x) != img){
            static thread_local cdb::SQ u1("UPDATE Tags SET icon=? WHERE id=?");
            static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }

    void    u_tag_notify(Fragment frag,Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                u_tag_erase(doc);
                return ;
            }
        }
        
        bool    created = false;
        Tag     x   = cdb::db_tag(doc, &created);
        if(!x)
            return ;
        
        cdb_options_t   opts    = cdb::IS_UPDATE|cdb::U_INFO|cdb::U_LEAF;
        if(created)
            opts |= cdb::U_ICON;
        

        Tag::SharedData data = u_tag_update(x, opts);
        if(!data){
            yWarning() << "Unable to update tag '" << cdb::key(x) << "'";
            return ;
        }
    }
    
    void    u_tag_notify_icons(Fragment frag,Change)
    {
        u_tag_icon(cdb::tag(cdb::document(frag), true));
    }

    void    u_tag_stage3(Document doc) 
    {
        Tag     x       = cdb::db_tag(doc);
        Tag::SharedData data = u_tag_update(x, cdb::DONT_LOCK|cdb::U_INFO|cdb::U_ICON);
        if(!data){
            yWarning() << "Unable to initialize tag '" << cdb::key(x) << "'";
            return ;
        }
    }
    
    void    u_tag_stage3_leaf(Document doc)
    {
        Tag     x       = cdb::db_tag(doc);
        Tag::SharedData data = u_tag_update(x, cdb::DONT_LOCK|cdb::U_LEAF);
        if(!data){
            yWarning() << "Unable to set tag leaf '" << cdb::key(x) << "'";
            return ;
        }
    }

    Tag::SharedData     u_tag_update(Tag x, cdb_options_t opts)
    {
        if(!x)
            return Tag::SharedData();

        if(opts & cdb::U_ICON)
            u_tag_icon(x);

        auto data  = cdb::merged(x, opts);
        if(!data)
            return Tag::SharedData();
         
        static thread_local cdb::SQ uInfo("UPDATE Tags SET name=?,brief=? WHERE id=?");
        static thread_local cdb::SQ uLeaf("UPDATE Tags SET leaf=? WHERE id=?");
        
        if(opts&cdb::U_INFO){
            uInfo.bind(1, data->name);
            uInfo.bind(2, data->brief);
            uInfo.bind(3, x.id);
            uInfo.exec();
        }
        if(opts&cdb::U_LEAF){
            Leaf    l   = cdb::leaf(data->leaf);
            uLeaf.exec(l.id, x.id);
        }
        
        return data;
    }
    
}
