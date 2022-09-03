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
  void    leaf_icons(Fragment frag)
    {
        if(hidden(folder(frag)))
            return;
        update_icon(leaf(document(frag), true));
    }

    void    leaf_stage3(Document doc)
    {
        if(hidden(folder(doc)))
            return;
        Leaf            x   = db_leaf(doc);
        Leaf::SharedData data   = update(x, DONT_LOCK|U_INFO|U_ICON|U_TAGS);
        if(!data){
            yWarning() << "Unable to initialize leaf '" << key(x) << "'";
            return ;
        }

        auto rep        = diff::additions(doc, data->attrs);
        
        
        rep.exec_inserts();

        // do the rest...later
    }
    
    void    leaf_update(Fragment frag, Change chg)
    {
        if(hidden(folder(frag)))
            return;

        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(leaf(doc));
                return ;
            }
        }
        
        bool    created = false;
        Leaf     x   = db_leaf(doc, &created);
        if(!x)
            return ;
        
        cdb_options_t   opts    = IS_UPDATE|U_INFO|U_TAGS;
        if(created)
            opts |= U_ICON;
        

        Leaf::SharedData data = update(x, opts);
        if(!data){
            yWarning() << "Unable to update leaf '" << key(x) << "'";
            return ;
        }
        
        if(created){
            //  MORE
        } else {
            //  MORE TODO 
            auto rep     = diff::changes(doc, data->attrs);
            rep.exec_inserts();
            rep.exec_reindex();
            rep.exec_removals();
        }
        
    }
}
