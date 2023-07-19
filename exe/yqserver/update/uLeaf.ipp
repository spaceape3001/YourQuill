////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uLeaf.hpp"
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/file/FileSpec.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    ULeaf&  ULeaf::get(Leaf x)
    {
        return U<Leaf>::lookup<ULeaf>(x);
    }

    std::pair<ULeaf&,bool>   ULeaf::create(Document doc)
    {
        bool created = false;
        Leaf    x   = cdb::db_leaf(doc,&created);
        return { get(x), created };
    }
    
    const FileSpec&  ULeaf::lookup()
    {
        static const FileSpec  s_ret(CACHE, "*.y");
        return s_ret;
    }

    void             ULeaf::notify(Fragment frag,Change chg)
    {
        auto [u,cr] = create(cdb::document(frag));
        u.reload();
        u.u_info();
        u.u_tags();
        if(cr)
            u.u_icon();
        // TODO
    }
    
    void             ULeaf::icons(Fragment frag,Change)
    {
        Leaf    x   = cdb::leaf(cdb::document(frag), true);
        if(!x)
            return ;
        get(x).u_icon();
    }
    
    void             ULeaf::s3(Document doc)
    {
        auto [u,cr] = create(doc);
        u.reload();
        u.u_info();
        u.u_tags();
        u.u_icon();
    }

    ////////////////////////////////////////////////////////////////////////////

    ULeaf::ULeaf(Leaf x) : U<Leaf>(x, cdb::key(x)), 
        doc(cdb::document(x)), 
        atom(cdb::db_atom(doc))
    {
    }

    void    ULeaf::reload()
    {
        if(!x)
            return ;
        def     = cdb::merged(x, { cdb::IS_UPDATE | cdb::IGNORE_CONTEXT });
    }

    void    ULeaf::u_icon()
    {
        if(!x)
            return ;

        static thread_local CacheQuery u1("UPDATE Leafs SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");
        static thread_local CacheQuery u3("UPDATE Atoms SET icon=? WHERE id=?");

        Image       img     = cdb::best_image(doc);
        if(img != icon){
                // we'll eventually do inheritance....
            u1.exec(img.id, id);
            u2.exec(img.id, doc.id);
            u3.exec(img.id, atom.id);
            icon        = img;
        }
    }

    void    ULeaf::u_info()
    {
        if(!x)
            return;
        if(!def)
            return;
            
        static thread_local CacheQuery uInfo("UPDATE Leafs SET title=?, abbr=?, brief=? WHERE id=?");
        std::string_view title   = def->title();
        std::string_view abbr    = def->abbr();
        std::string_view brief   = def->brief();
        if(title.empty())
            title       = def->attrs.value(kv::key({"nick", "name"}));
        
        if(title.empty()){
            uInfo.bind(1, key);     // fall back (for now) 
                                    // TODO ... make this smarter (later)
        } else 
            uInfo.bind(1, title);
        uInfo.bind(2, abbr);
        uInfo.bind(3, brief);
        uInfo.bind(4, id);
        uInfo.exec();
    }

    void    ULeaf::u_tags()
    {
        if(!x)
            return ;
        if(!def)
            return ;
    
        TagSet  new_tags = cdb::find_tags_set(def->tags(), true);
        auto    chg = add_remove(tags, new_tags);
        
        static thread_local CacheQuery dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
        static thread_local CacheQuery iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
        
        for(Tag t : chg.added)
            iTag.exec(id, t.id);
        for(Tag t : chg.removed)
            dTag.exec(id, t.id);
        tags    = new_tags;
    }

    void    ULeaf::x_erase()
    {
        static thread_local CacheQuery  stmts[] = {
            CacheQuery( "DELETE FROM LTags WHERE leaf=?" ),
            CacheQuery( "DELETE FROM Leafs WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(id);
    }
}

