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

    Leaf::SharedData         update_this(Leaf x, cdb_options_t opts);
    
    void    u_leaf_change(Document doc, cdb_options_t opts)
    {
        bool    created = false;
        Leaf        x   = (opts & REMOVED) ? cdb::leaf(doc) : cdb::db_leaf(doc, &created);
        if(!x)
            return ;
            
        created = created || (opts & STARTUP);
        if(created)
            opts |= U_ICON | CREATED;
        
        Leaf::SharedData data   = update_this(x, opts|U_INFO);
        if(!data){
            yWarning() << "Unable to initialize leaf '" << key(x) << "'";
            return ;
        }
        
        Attribute::Report   rep;
        Atom                xa;
        
        if(created){
            rep             = diff::additions(doc, data->attrs);
            rep.exec_inserts();
            xa              = db_atom(doc);
        } else {
            rep             = diff::changes(doc, data->attrs);
            rep.exec_inserts();
            rep.exec_reindex();
            rep.exec_removals();
            xa              = atom(doc);
            
        }
        
        std::set<Class>     clsset = make_set(db_classes(data->classes()));
        update_atom(xa, doc, rep, opts, clsset);
        
        if(created)
            u_leaf_icon(x);

        if(opts & REMOVED){
            if(fragments_count(doc) <= 1)
                u_leaf_erase(x);
        }
    }

    Leaf::SharedData         update_this(Leaf x, cdb_options_t opts)
    {
        if(!x)
            return Leaf::SharedData();
        
        if(opts & U_ICON)
            u_leaf_icon(x);
        
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
    
    void    u_leaf_stage3(Document doc)
    {
        if(hidden(folder(doc)))
            return;
        
        u_leaf_change(doc, STARTUP | DONT_LOCK | U_TAGS );
    }
    
    void    u_leaf_update(Fragment frag, Change chg)
    {
        if(hidden(folder(frag)))
            return;
        cdb_options_t   opts    = IS_UPDATE | U_TAGS;
        if(chg == Change::Removed)
            opts |= REMOVED;
        
        u_leaf_change(document(frag), opts);
    }
}


namespace yq {
    void    u_leaf_erase(Leaf x)
    {
        static thread_local cdb::SQ  stmts[] = {
            cdb::SQ( "DELETE FROM LTags WHERE leaf=?" ),
            cdb::SQ( "DELETE FROM Leafs WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }

    void    u_leaf_erase(Document doc)
    {
        u_leaf_erase(cdb::leaf(doc));
    }
    
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

    void    u_leaf_notify_icons(Fragment frag, Change)
    {
        u_leaf_icon(cdb::leaf(cdb::document(frag), true));
    }
        
    void    u_leaf_stage3_pass1_declare(Document doc)
    {
        Leaf    xl  = cdb::db_leaf(doc);
        Atom    xa  = cdb::db_atom(doc);
        
        //  do more
    }
}
