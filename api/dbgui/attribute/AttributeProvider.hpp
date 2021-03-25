////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/Attribute.hpp>
#include <gui/Provider.hpp>

using AttributeProvider = std::shared_ptr<Provider<Attribute>>;

namespace provider {
    AttributeProvider   all_attributes();
    AttributeProvider   all_attributes(Document);
    AttributeProvider   attributes(Attribute);
    AttributeProvider   attributes(Document);
}
