////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "update/uFwd.hpp"

#include <db/AtomSys.hpp>
#include <db/Image.hpp>
#include <db/Leaf.hpp>
#include <db/Tag.hpp>


//static constexpr const std::initializer_list<const char*>    kImages = { "svg", "png", "gif", "jpg", "bmp", "tif", "tiff" };

    /*
        These are the data structures to persist (non-DB) during a run.  They won't be deleted until next run.
        
        (They're also a crutch until the database gets super-good)
    */
// These are our tracking data structures




Image           calc_icon_for(Folder, const QString&);
Image           calc_icon_for(Field);
Image           calc_icon_for(const Root*);

void            update_field_icon(Fragment);



