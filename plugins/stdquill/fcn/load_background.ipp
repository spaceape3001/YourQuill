////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

bool        load_background(unsigned int opts)
{
    Document    doc = cdb::first_document(gBackgroundFiles);
    bool    now = false;

    if(doc){
        do {
            Fragment    frag = cdb::fragment(doc, DataRole::Image);
            if(!frag)
                break;
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK))
                lk      = Fragment::Lock::read(frag);
            Ref<TypedBytes> tb  = TypedBytes::load(cdb::path(frag));
            if(!tb)
                break;
            gBackground = tb;
            now = true;
        } while(false);
    }

    return gHasBackground.exchange(now) != now;
}
