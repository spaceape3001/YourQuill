////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"

struct UImage { // using the same scoping as the rest.
    static void init_scan();
    static void on_image_add(Fragment);
    static void on_image_modified(Fragment);
    static void on_image_removed(Fragment);
    static void update(Image);
};

