////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

 #pragma once
 
 namespace {
 
    void    update_tag(Document doc)
    {
        Tag         x   = db_tag(doc);
        if(!x)
            return ;

        Tag::SharedData data = update_info(x);
        if(!data){
            yWarning() << "Unable to update tag '" << key(x) << "' due to lack of data";
            return ;
        }

        update(x, leaf(data->leaf));
    }
 
    void    change_tag(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(tag(doc));
                return ;
            }
        }
        
        update_tag(doc);
    }
 
    void    change_tag_image(Fragment frag, Change)
    {
        update_icon(tag(document(frag), true));
    }
 
    //  TODO images & leaf....
 
    YQ_INVOKE( 
        u_change<change_tag>(tags_folder(), "*.tag"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_tag_image>(tags_folder(), z);
    )
 }
 
