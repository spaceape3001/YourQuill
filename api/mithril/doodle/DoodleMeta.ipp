////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include <mithril/doodle/DoodleCDB.hpp>

namespace {
    void reg_doodle_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Doodle>();
            w.property("id", READ_ONLY, &Doodle::id);
            w.property("key", (std::string(*)(Doodle)) cdb::key);
        }
    }
}
