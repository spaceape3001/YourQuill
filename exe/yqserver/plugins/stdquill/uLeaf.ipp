////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/leaf/LeafData.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

#include <update/uAtom.hpp>

using namespace yq;
using namespace yq::mithril;
using namespace yq::mithril::update;

//#include "uAtom.hpp"

namespace {

    /*

        LEAF is a markdown document with attributes defined at the top.
        
        LEAF will posit an atom, the defacto "home" for a (big/important) scope atom, 
        however, an atom can be defined elsewhere.

    */



    #if 0
    void         u_leaf(Document doc, cdb_options_t opts)
    {
        bool    created = false;
        Leaf        x   = (opts & REMOVED) ? cdb::leaf(doc) : cdb::db_leaf(doc, &created);
        if(!x)
            return ;
        
        created = created || (opts & STARTUP);
        if(created)
            opts |= U_ICON | CREATED;

        if(opts & U_ICON)
            u_leaf_icon(x);

        auto dp  = merged(x, opts|IS_UPDATE);
        if(!dp){
            yWarning() << "Unable to load/merge leaf data for '" << key(x) << "'";
            return;
        }
        
        static thread_local CacheQuery uInfo("UPDATE Leafs SET title=?, abbr=?, brief=? WHERE id=?");
        if(opts & U_INFO){
            std::string_view title   = dp->title();
            std::string_view abbr    = dp->abbr();
            std::string_view brief   = dp->brief();
            if(title.empty())
                title       = dp->attrs.value(kv::key({"nick", "name"}));
            
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
        
        static thread_local CacheQuery dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
        static thread_local CacheQuery iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
        
        if(opts & U_TAGS){
            std::set<Tag>   old_tags = tags_set(x);
            std::set<Tag>   new_tags = find_tags_set(dp->tags(), true);

            auto ch_tag = add_remove(old_tags, new_tags);
            iTag.batch(x.id, ids_for(ch_tag.added));
            dTag.batch(x.id, ids_for(ch_tag.removed));
        }

        Attribute::Report   rep;
        Atom                xa;
        
        if(created){
            xa              = db_atom(doc);
        } else {
            xa              = find_atom(doc);
        }
        
        if(opts & U_ATTRIBUTES){
            if(created){
                rep             = diff::additions(doc, dp->attrs);
                rep.exec_inserts();
            } else {
                rep             = diff::changes(doc, dp->attrs);
                rep.exec_inserts();
                rep.exec_reindex();
                rep.exec_removals();
            }

            std::set<Class>     clsset = make_set(cdb::db_classes(dp->classes()));
            u_atom(xa, doc, rep, opts, clsset);

            if(opts & REMOVED){
                if(cdb::fragments_count(doc) <= 1)
                    u_leaf_erase(x);
            }
        }
    }
    #endif

    #if 0
    void    u_leaf_erase(Leaf x)
    {
        static thread_local CacheQuery  stmts[] = {
            CacheQuery( "DELETE FROM LTags WHERE leaf=?" ),
            CacheQuery( "DELETE FROM Leafs WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
    #endif

    #if 0
    //  UNUSED
    void    u_leaf_erase(Document doc)
    {
        u_leaf_erase(cdb::leaf(doc));
    }
    #endif

    void    u_leaf_icon(Leaf x)
    {
        if(!x)
            return ;
            
        Document    doc     = cdb::document(x);
        Image       img     = cdb::best_image(doc);
        static thread_local CacheQuery u1("UPDATE Leafs SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

        if(cdb::icon(x) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }

    void    u_leaf_notify(Fragment frag, Change chg)
    {
        if(cdb::hidden(cdb::folder(frag)))
            return;
        cdb_options_t   opts    = cdb::IS_UPDATE | cdb::U_TAGS | cdb::U_ATTRIBUTES | cdb::U_INFO;
        if(chg == Change::Removed)
            opts |= cdb::REMOVED;
        
        Document    doc = cdb::document(frag);
        Leaf        x   = cdb::db_leaf(cdb::document(frag));
        
        auto dp   = cdb::merged(x, cdb::IS_UPDATE | cdb::DONT_LOCK);
        if(!dp){
            yWarning() << "Unable to load leaf data for '" << cdb::key(doc) << "'";
            return ;
        }
        
        UAtom::changed(dp->attrs, doc);
        
        //u_atom(dp->attrs, doc);
        
        //  if there's any context stuff, do it here.....
    }

    void    u_leaf_notify_icons(Fragment frag, Change)
    {
        u_leaf_icon(cdb::leaf(cdb::document(frag), true));
    }
    
    void    u_leaf_title(Leaf x, Leaf::SharedData dp)
    {
        auto t = dp->title();
        if(t != cdb::title(x)){
            static thread_local CacheQuery u("UPDATE Leafs SET title=? WHERE id=?");
            auto u_af = u.af();
            u.bind(1, t);
            u.bind(2, x.id);
            u.exec();
        }
    }
        
    void    s3_leaf_pass1(Document doc)
    {
        if(cdb::hidden(cdb::folder(doc)))
            return;
        
        Leaf    x   = cdb::db_leaf(doc);
        if(!x){
            yWarning() << "Unable to create leaf for '" << cdb::key(doc) << "'";
            return;
        }

        u_leaf_icon(x);
        
        auto dp   = cdb::merged(x, cdb::IS_UPDATE | cdb::DONT_LOCK);
        if(!dp){
            yWarning() << "Unable to load leaf data for '" << cdb::key(doc) << "'";
            return ;
        }
        
        u_leaf_title(x, dp);
        
        Atom xa = UAtom::s3_create(dp->attrs, doc);
        
        //Atom xa = s3_atom_create(dp->attrs, doc);
        UAtom::s3_bind(doc);
        UAtom::s3_notify(doc);
        //s3_atom_notify(xa);
        
        static thread_local CacheQuery u("UPDATE Leafs SET atom=? WHERE id=?");
        auto u_af = u.af();
        u.bind(1, xa.id);
        u.bind(2, x.id);
        u.exec();
    }

    void    s3_leaf_pass2(Document doc)
    {
        if(cdb::hidden(cdb::folder(doc)))
            return;
        
        Atom xa = cdb::find_atom(doc);
        if(!xa)
            return;
            
        UAtom::s3_bind(doc);
        UAtom::s3_notify(doc);

        //s3_atom_bind(xa);
        //s3_atom_notify(xa);
    }
}
