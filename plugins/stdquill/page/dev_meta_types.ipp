////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfo.hpp>

namespace {

    void page_dev_meta_types(WebHtml&h)
    {
        h.title() << "All Type Infos";
        dev_table(h, TypeInfo::all());
    }
    
    YQ_INVOKE( reg_webpage<page_dev_meta_types>("/dev/meta/types"); )
    
}
