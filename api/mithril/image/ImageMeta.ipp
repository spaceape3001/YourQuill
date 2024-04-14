////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include "ImageCDB.hpp"

namespace {
    void reg_image_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Image>();
            w.property("id", READ_ONLY, &Image::id);
            w.property("key", (std::string(*)(Image)) cdb::key);
        }
    }
}
