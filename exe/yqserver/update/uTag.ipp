////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uTag.hpp"
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    UTag&  UTag::get(Tag x)
    {
        return lookup<UTag, 512>(x);
    }

    std::pair<UTag&, bool>  UTag::create(Document doc)
    {
        bool created = false;
        Tag    x   = cdb::db_tag(doc,&created);
        return { get(x), created };
    }

    void    UTag::icons(Fragment frag,Change)
    {
        Tag    x   = cdb::find_tag(cdb::document(frag), true);
        if(!x)
            return ;
        get(x).u_icon();
    }
    

    void    UTag::notify(Fragment frag,Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Tag    x   = cdb::find_tag(doc);
                if(x){
                    get(x).x_erase();
                }
                return ;
            }
        }
        
        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_leaf();
        if(cr)
            u.u_icon();
    }
    
    void    UTag::s3(Document doc)
    {
        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_icon();
    }
    
    void    UTag::s3_leaf(Document doc)
    {
        auto [u,cr] = create(doc);
        u.u_leaf();
    }



    ////////////////////////////////////////////////////////////////////////////////

    UTag::UTag(Tag x) : U<Tag>(x, cdb::key(x)), doc(cdb::document(x))
    {
    }

    void    UTag::reload()
    {
        def = cdb::merged(x, cdb::DONT_LOCK|cdb::IS_UPDATE);
    }
    
    void    UTag::u_icon()
    {
        if(!x)
            return ;
            
        static thread_local CacheQuery u1("UPDATE Tags SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

        Image       img     = cdb::best_image(doc);
        if(img != icon){
            u1.exec(img.id, id);
            u2.exec(doc.id, id);
            icon    = img;
        }
    }
    
    void    UTag::u_info()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        static thread_local CacheQuery uInfo("UPDATE Tags SET name=?,brief=? WHERE id=?");
        uInfo.bind(1, def->name);
        uInfo.bind(2, def->brief);
        uInfo.bind(3, id);
        uInfo.exec();
    }
    
    void    UTag::u_leaf()
    {
        if(!x)
            return ;
        if(!def)
            return ;

        static thread_local CacheQuery uLeaf("UPDATE Tags SET leaf=? WHERE id=?");
        Leaf l      = cdb::leaf(def->leaf);
        if(l != leaf){
            uLeaf.exec(l.id, id);
            leaf    = l;
        }
    }

    void    UTag::x_erase()
    {
        static thread_local CacheQuery  stmts[] = {
            CacheQuery( "DELETE FROM CTags WHERE tag=?" ),
            CacheQuery( "DELETE FROM FTags WHERE tag=?" ),
            CacheQuery( "DELETE FROM LTags WHERE tag=?" ),
            CacheQuery( "DELETE FROM Tags WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
}

