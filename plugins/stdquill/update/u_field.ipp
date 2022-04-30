////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    update_field(Document doc)
    {
        Field               x = cdb::db_field(doc);
        if(!x)
            return ;
        
        Field::SharedData   data = cdb::update_info(x);
        if(!data){
            yWarning() << "Unable to update field '" << key(x) << "' due to lack of data";
            return ;
        }

        //  Now it's time for tags/etc
    }
    
    void    change_field(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(field(doc));
                return ;
            }
        }
        update_field(doc);
    }
    
    void    change_field_image(Fragment frag, Change)
    {
        update_icon(field(document(frag), true));
    }    
    
    YQ_INVOKE( 
        u_change<change_field>(fields_folder(), "*.field");
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_field_image>(fields_folder(), z);
        
    )
    
}
