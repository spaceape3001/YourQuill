////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_dev_meta_objects(WebHtml&h)
    {
        h.title() << "All Meta Objects";
        dev_table(h, ObjectInfo::all());
    }
    
    YQ_INVOKE( reg_webpage<page_dev_meta_objects>("/dev/meta/objects"); )
    
}
