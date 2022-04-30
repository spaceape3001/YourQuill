////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    update_class(Document doc)
    {
        Class       x   = db_class(doc);
        if(!x)
            return ;
            
        Class::SharedData   data    = update_info(x);
        if(!data){
            yWarning() << "Unable to update user '" << key(x) << "' due to lack of data";
            return ;
        }
    }
    
    void    change_class(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(class_(doc));
                return ;
            }
        }
        
        update_class(doc);
    }
    
    void    change_class_image(Fragment frag, Change)
    {
        update_icon(class_(document(frag), true));
    }    
    
    YQ_INVOKE( 
        u_change<change_class>(classes_folder(), "*.class"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_class_image>(classes_folder(), z);
    )
    
}
