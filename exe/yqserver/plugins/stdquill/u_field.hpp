////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/enum/Change.hpp>
#include <kernel/preamble.hpp>

namespace {
    
    //! Erases the field from the cache
    void    u_field_erase(Field);
    void    u_field_erase(Document);

    void    u_field_icon(Field);
    
    void    u_field_notify(Fragment, Change);
    void    u_field_notify_icons(Fragment, Change);
    
    void    u_field_stage3(Document);
}
