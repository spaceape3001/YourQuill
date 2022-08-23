////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    category_icons(Fragment frag, Change)
    {
        update_icon(category(document(frag), true));
    }
    
    void    category_stage3(Document doc)
    {
        Category    x       = db_category(doc);
        Category::SharedData data = update(x, DONT_LOCK|U_INFO|U_ICON|IS_UPDATE);
        if(!data){
            yWarning() << "Unable to update category '" << key(x) << "' due to lack of data";
            return ;
        }
    }
    
    void    category_update(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(category(doc));
                return ;
            }
        }
        
        bool created = false;
        Category         x   = db_category(doc, &created);
        if(!x)
            return ;

        cdb_options_t       opts    = U_INFO | IS_UPDATE;
        if(created)
            opts |= U_ICON;

        Category::SharedData data = update(x, opts);
        if(!data){
            yWarning() << "Unable to update category '" << key(x) << "' due to lack of data";
            return ;
        }
    }
    

}
