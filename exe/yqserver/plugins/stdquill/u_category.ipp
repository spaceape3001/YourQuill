////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "u_category.hpp"

#include <basic/Logging.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/org/CategoryCDB.hpp>
#include <kernel/org/CategoryData.hpp>

namespace yq {
    struct Category;
    
    void    u_category_erase(Category x)
    {
        static thread_local cdb::SQ  stmts[] = {
            cdb::SQ( "UPDATE Classes SET category=0 WHERE category=?" ),
            cdb::SQ( "DELETE FROM Categories WHERE id=?" )
        };
        
        if(!x)
            return ;
        
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
    
    void    u_category_erase(Document doc)
    {
        u_category_erase(cdb::category(doc));
    }

    void    u_category_icon(Category x)
    {
        if(!x)
            return ;

        Document    doc     = cdb::document(x);
        Image       img     = cdb::best_image(doc);
        
        if(img != cdb::icon(x)){
            static thread_local cdb::SQ u1("UPDATE Categories SET icon=? WHERE id=?");
            static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");
            
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }

    void    u_category_notify(Fragment frag,Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                u_category_erase(doc);
                return ;
            }
        }
        
        bool created = false;
        Category         x   = cdb::db_category(doc, &created);
        if(!x)
            return ;

        cdb_options_t       opts    = cdb::U_INFO | cdb::IS_UPDATE;
        if(created)
            opts |= cdb::U_ICON;

        Category::SharedData data = u_category_update(x, opts);
        if(!data){
            yWarning() << "Unable to update category '" << cdb::key(x) << "' due to lack of data";
            return ;
        }
    }
    
    void    u_category_notify_icons(Fragment frag,Change)
    {
        u_category_icon(cdb::category(cdb::document(frag), true));
    }
    

    void    u_category_stage3(Document doc)
    {
        Category    x       = cdb::db_category(doc);
        Category::SharedData data = u_category_update(x, cdb::DONT_LOCK|cdb::U_INFO|cdb::U_ICON|cdb::IS_UPDATE);
        if(!data){
            yWarning() << "Unable to update category '" << cdb::key(x) << "' due to lack of data";
            return ;
        }
    }

    Category::SharedData    u_category_update(Category x, cdb_options_t opts)
    {
        if(!x)
            return Category::SharedData();

        if(opts & cdb::U_ICON)
            u_category_icon(x);
        
        Category::SharedData data = cdb::merged(x, opts);
        if(!data)
            return Category::SharedData();
            
        if(opts & cdb::U_INFO){
            static thread_local cdb::SQ u("UPDATE Categories SET name=?,brief=? WHERE id=?");
            u.bind(1, data->name);
            u.bind(2, data->brief);
            u.bind(3, x.id);
            u.exec();
        }
        return data;
    }
}
