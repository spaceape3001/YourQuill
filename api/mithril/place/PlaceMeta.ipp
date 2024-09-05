////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "PlaceCDB.hpp"

namespace {
    void reg_place_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Place>();
            w.property("id", READ_ONLY, &Place::id);
            w.property("key", (std::string(*)(Place)) cdb::key);
        }
    }
}
