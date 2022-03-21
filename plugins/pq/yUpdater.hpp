////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"

//#include <yq/Atom.hpp>
//#include <yq/Class.hpp>
//#include <yq/Image.hpp>
//#include <yq/Leaf.hpp>
//#include <yq/Tag.hpp>


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



