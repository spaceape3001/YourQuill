////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

 #pragma once
 
 namespace {
 
    void    update_category(Document doc)
    {
        Category         x   = db_category(doc);
        if(!x)
            return ;

        Category::SharedData data = update_info(x);
        if(!data){
            yWarning() << "Unable to update category '" << key(x) << "' due to lack of data";
            return ;
        }
    }
 
    void    change_category(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(category(doc));
                return ;
            }
        }
        
        update_category(doc);
    }
 
    void    change_category_image(Fragment frag, Change)
    {
        update_icon(category(document(frag), true));
    }
 
    //  TODO images & leaf....
 
    YQ_INVOKE( 
        u_change<change_category>(categories_folder(), "*.cat"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_category_image>(categories_folder(), z);
    )
 }
 
