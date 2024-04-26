////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>

namespace {
    void reg_graphviz_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Graphviz>();
            w.property("id", READ_ONLY, &Graphviz::id);
            w.property("key", (std::string(*)(Graphviz)) cdb::key);
        }
    }
}
