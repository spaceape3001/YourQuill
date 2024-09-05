////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/typedef/json.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    json json_(Document);
    json json_(const DocumentVector&);
}
