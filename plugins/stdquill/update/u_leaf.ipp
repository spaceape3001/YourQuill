////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*

    LEAF is a markdown document with attributes defined at the top.
    
    LEAF will posit an atom, the defacto "home" for a (big/important) scope atom, 
    however, an atom can be defined elsewhere.

*/

namespace {
    
    void    leaf_change(Document doc, cdb_options_t opts)
    {
        bool    created = false;
        Leaf        x   = (opts & REMOVED) ? leaf(doc) : db_leaf(doc, &created);
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
            update::leaf_icon(x);

        if(opts & REMOVED){
            if(fragments_count(doc) <= 1)
                update::leaf_erase(x);
        }
    }

    void    leaf_stage3(Document doc)
    {
        if(hidden(folder(doc)))
            return;
        
        leaf_change(doc, STARTUP | DONT_LOCK | U_TAGS );
    }
    
    void    leaf_update(Fragment frag, Change chg)
    {
        if(hidden(folder(frag)))
            return;
        cdb_options_t   opts    = IS_UPDATE | U_TAGS;
        if(chg == Change::Removed)
            opts |= REMOVED;
        
        leaf_change(document(frag), opts);
    }
}
