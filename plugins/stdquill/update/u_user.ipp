////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    user_icons(Fragment frag)
    {
        update_icon(user(document(frag), true));
    }

    void    user_stage3(Document doc)
    {
        User    x   = db_user(doc);
        User::SharedData data = update_this(x, DONT_LOCK|U_INFO|U_ICON);
        if(!data){
            yWarning() << "Unable to initialize user '" << key(x) << "'";
            return ;
        }
    }
    
    void    user_update(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(user(doc));
                return ;
            }
        }

        bool    created = false;
        User    x   = db_user(doc, &created);
        if(!x)
            return ;
            
        cdb_options_t   opts = IS_UPDATE|U_INFO;
        if(created)
            opts |= U_ICON;

        User::SharedData data = update_this(x, opts);
        if(!data){
            yWarning() << "Unable to update user '" << key(x) << "'";
            return ;
        }
    }}
