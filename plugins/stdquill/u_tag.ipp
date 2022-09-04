////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*
    This file provides the hooks that calls the update in uTag, binding it from the registration.
*/

namespace {
    void    tag_stage3(Document doc)
    {
        Tag     x       = db_tag(doc);
        Tag::SharedData data = update(x, DONT_LOCK|U_INFO|U_ICON);
        if(!data){
            yWarning() << "Unable to initialize tag '" << key(x) << "'";
            return ;
        }
    }
    
    void    tag_stage3_leaf(Document doc)
    {
        Tag     x       = db_tag(doc);
        Tag::SharedData data = merged(x, DONT_LOCK|U_LEAF);
        if(!data){
            yWarning() << "Unable to set tag leaf '" << key(x) << "'";
            return ;
        }
    }

    void    tag_icons(Fragment frag)
    {
        update_icon(tag(document(frag), true));
    }
    
    void    tag_update(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(tag(doc));
                return ;
            }
        }
        
        bool    created = false;
        Tag     x   = db_tag(doc, &created);
        if(!x)
            return ;
        
        cdb_options_t   opts    = IS_UPDATE|U_INFO|U_LEAF;
        if(created)
            opts |= U_ICON;
        

        Tag::SharedData data = update(x, opts);
        if(!data){
            yWarning() << "Unable to update tag '" << key(x) << "'";
            return ;
        }
    }
}
