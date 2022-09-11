////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    image_stage3(Document doc)
    {
        for(Fragment frag : fragments(doc))
            update_image(db_image(frag), DONT_LOCK|IS_UPDATE);
    }

    void    image_update(Fragment frag, Change chg)
    {
        switch(chg){
        case Change::Added:
        case Change::Modified:
            update_image(db_image(frag));
            break;
        case Change::Removed:
            erase(image(frag));
            break;
        default:
            break;
        }
    }
}
