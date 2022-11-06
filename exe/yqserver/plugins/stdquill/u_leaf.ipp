////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "u_leaf.hpp"

#include <basic/Logging.hpp>
#include <kernel/atom/AtomCDB.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/leaf/LeafCDB.hpp>
#include <kernel/leaf/LeafData.hpp>


/*

    LEAF is a markdown document with attributes defined at the top.
    
    LEAF will posit an atom, the defacto "home" for a (big/important) scope atom, 
    however, an atom can be defined elsewhere.

*/

namespace {

    struct InitLeaf {
        Leaf::SharedData    data;
        Atom                atom;
        Attribute::Report   attrs;
    };
    
    static std::map<Leaf, InitLeaf>    iLeafs;


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
        
        static thread_local SQ uInfo("UPDATE Leafs SET title=?, abbr=?, brief=? WHERE id=?");
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
        
        static thread_local SQ dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
        static thread_local SQ iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
        
        if(opts & U_TAGS){
            std::set<Tag>   old_tags = tags_set(x);
            std::set<Tag>   new_tags = tags_set(dp->tags(), true);

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

            std::set<Class>     clsset = make_set(db_classes(dp->classes()));
            u_atom(xa, doc, rep, opts, clsset);

            if(opts & REMOVED){
                if(fragments_count(doc) <= 1)
                    u_leaf_erase(x);
            }
        }
    }

    void    u_leaf_erase(Leaf x)
    {
        static thread_local cdb::SQ  stmts[] = {
            cdb::SQ( "DELETE FROM LTags WHERE leaf=?" ),
            cdb::SQ( "DELETE FROM Leafs WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }

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
        static thread_local cdb::SQ u1("UPDATE Leafs SET icon=? WHERE id=?");
        static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");

        if(cdb::icon(x) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }
    
    void    u_leaf_notify(Fragment frag, Change chg)
    {
        if(hidden(folder(frag)))
            return;
        cdb_options_t   opts    = IS_UPDATE | U_TAGS | U_ATTRIBUTES | U_INFO;
        if(chg == Change::Removed)
            opts |= REMOVED;
        
        u_leaf(document(frag), opts);
    }

    void    u_leaf_notify_icons(Fragment frag, Change)
    {
        u_leaf_icon(cdb::leaf(cdb::document(frag), true));
    }
        
    void    u_leaf_stage3_pass1(Document doc)
    {
        if(hidden(folder(doc)))
            return;
        
        Leaf    x   = cdb::db_leaf(doc);
        if(!x){
            yWarning() << "Unable to create leaf for '" << cdb::key(doc) << "'";
            return;
        }

        u_leaf_icon(x);
        
        InitLeaf&   ix  = iLeafs[x];
        ix.data     = cdb::merged(x, IS_UPDATE | DONT_LOCK);
        if(!ix.data){
            yWarning() << "Unable to load leaf data for '" << cdb::key(doc) << "'";
            return ;
        }
        
        

        u_leaf(doc, DONT_LOCK|STARTUP|U_TAGS|U_INFO);
    }
    
    void    u_leaf_stage3_pass2(Document doc)
    {
        if(hidden(folder(doc)))
            return;
        u_leaf(doc, DONT_LOCK|U_ATTRIBUTES);
    }

    void    u_leaf_stage4_cleanup()
    {
        iLeafs.clear();
    }
}
