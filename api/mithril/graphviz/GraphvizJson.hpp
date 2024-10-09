////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/typedef/json.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    json json_(Graphviz);
    json json_(const GraphvizVector&);
}
