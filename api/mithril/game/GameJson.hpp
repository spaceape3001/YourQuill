////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/typedef/json.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    json json_(Game);
    json json_(const GameVector&);
}
