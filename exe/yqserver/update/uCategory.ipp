////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uCategory.hpp"
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/file/FileSpec.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    UCategory&  UCategory::get(Category x)
    {
        return U<Category>::lookup<UCategory>(x);
    }

    std::pair<UCategory&, bool>  UCategory::create(Document doc)
    {
        bool created = false;
        Category    x   = cdb::db_category(doc,&created);
        return { get(x), created };
    }

    void             UCategory::icons(Fragment frag,Change chg)
    {
        Category    x   = cdb::category(cdb::document(frag), true);
        if(!x)
            return ;
        get(x).u_icon();
    }

    const FileSpec&  UCategory::lookup()
    {
        static const FileSpec s_ret(CACHE, cdb::categories_folder(), "*.cat");
        return s_ret;
    }

    void             UCategory::notify(Fragment frag,Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Category    x   = cdb::category(doc);
                if(x){
                    get(x).x_erase();
                }
                return ;
            }
        }
        
        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        if(cr)
            u.u_icon();
    }
    
    void             UCategory::s3(Document doc)
    {
        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_icon();
    }
    

    ////////////////////////////////////////////////////////////////////////////////

    UCategory::UCategory(Category x) : U<Category>(x, cdb::key(x)), doc(cdb::document(x))
    {
    }

    void  UCategory::reload()
    {
        def = cdb::merged(x, cdb::DONT_LOCK|cdb::IS_UPDATE);
    }

    void  UCategory::u_icon()
    {
        if(!x)
            return ;
            
        static thread_local CacheQuery u1("UPDATE Categories SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

        Image       img     = cdb::best_image(doc);
        if(img != icon){
            u1.exec(img.id, id);
            u2.exec(doc.id, id);
            icon    = img;
        }
    }

    void  UCategory::u_info()
    {
        if(!x)
            return ;
        if(!def)
            return ;
            
        static thread_local CacheQuery u("UPDATE Categories SET name=?,brief=? WHERE id=?");
        u.bind(1, def->name);
        u.bind(2, def->brief);
        u.bind(3, id);
        u.exec();
    }


    void  UCategory::x_erase()
    {
        if(!x)
            return ;

        static thread_local CacheQuery  stmts[] = {
            CacheQuery( "UPDATE Classes SET category=0 WHERE category=?" ),
            CacheQuery( "DELETE FROM Categories WHERE id=?" )
        };
        
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
}

