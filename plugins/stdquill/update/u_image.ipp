////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    remove_image(Image img)
    {
        if(!img)
            return ;

        static thread_local SQ u("DELETE FROM Images WHERE id=?");
        u.exec(img.id);
    }

    void    u_image(Fragment frag, Change chg)
    {
        switch(chg){
        case Change::Added:
        case Change::Modified:
            update_image(db_image(frag));
            break;
        case Change::Removed:
            remove_image(db_image(frag));
            break;
        default:
            break;
        }
    }
    
    YQ_INVOKE(
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<u_image>(z);
    )
    
}
