////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mithril/id/Id.hpp>

namespace yq::mithril::provider {
    IdProvider  all_attributes();
    IdProvider  all_attributes(Document);
    IdProvider  attributes(Document);
    IdProvider  child_attributes(Attribute);
}
