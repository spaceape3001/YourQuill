////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    s3_image(Document doc)
    {
        for(Fragment frag : fragments(doc))
            update_image(db_image(frag), DONT_LOCK);
    }
    
    YQ_INVOKE( 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_stage3<s3_image>(z);
    )
    
}
