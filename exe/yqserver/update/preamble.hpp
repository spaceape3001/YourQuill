////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>
#include <math/Counter.hpp>

namespace yq::mithril {
    struct Class;
    struct Field;
}

namespace yq::mithril::update {
    using StringCountMap        = std::map<std::string,CountU16,IgCase>;
    using ClassCountMap         = std::map<Class,CountU16>;
    using FieldCountMap         = std::map<Field,CountU16>;
    
    struct UClass;
    struct UField;
}
