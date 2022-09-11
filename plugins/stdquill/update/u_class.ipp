////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    class_icons(Fragment frag)
    {
        update_icon(class_(document(frag), true));
    }

    //void    class_s3_create(Document doc)
    //{
        //db_class(doc);
    //}

    void    class_s3_init(Document doc)
    {
        Class x = db_class(doc);

        Class::SharedData       data    = update(x, DONT_LOCK|IS_UPDATE|U_INFO|U_ICON|U_TAGS);
        if(!data)
            yWarning() << "Unable to update class '" << key(x) << "'";
    }
    
    void    class_update(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(class_(doc));
                return ;
            }
        }

        bool        created = false;
        Class       x   = db_class(doc, &created);
        if(!x)
            return ;
            
        cdb_options_t       opts    = U_INFO|U_TAGS|IS_UPDATE;
        if(created)
            opts |= U_ICON;
            
        Class::SharedData   data    = update(x, opts);
        if(!data){
            yWarning() << "Unable to update class '" << key(x) << "'";
            return ;
        }
    }
}
