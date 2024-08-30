////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/config/json_fwd.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    json json_(User);
    json json_(const UserVector&);
}
