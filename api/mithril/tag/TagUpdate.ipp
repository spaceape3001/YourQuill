////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/document/DocumentCDB.hpp>
#include <mithril/file/FileSpec.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/tag/TagUpdate.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    std::pair<UTag&, bool>  UTag::create(Document doc)
    {
        bool created = false;
        Tag    x   = cdb::db_tag(doc,&created);
        return { get(x), created };
    }

    UTag&  UTag::get(Tag x)
    {
        return U<Tag>::lookup<UTag>(x);
    }

    void    UTag::icons(Fragment frag, [[maybe_unused]] Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UTag::icons('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif

        Tag    x   = cdb::find_tag(cdb::document(frag), true);
        if(!x)
            return ;
        get(x).u_icon();
    }
    
    const FileSpec&  UTag::lookup()
    {
        static const FileSpec s_ret(CACHE, cdb::tags_folder(), "*.tag");
        return s_ret;
    }

    void    UTag::notify(Fragment frag, Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UTag::notify('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif

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
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UTag::s3('" << cdb::key(doc) << "')";
        #endif

        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_icon();
    }
    
    void    UTag::s3_leaf(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UTag::s3_leaf('" << cdb::key(doc) << "')";
        #endif

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

