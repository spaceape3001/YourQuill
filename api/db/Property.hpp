////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CacheFwd.hpp"
#include <db/enum/AttrKind.hpp>

struct Property {
    uint64_t    id;
    constexpr auto    operator<=>(const Property&rhs) const = default; 
    constexpr operator bool() const { return id != 0ULL; }
};

namespace cdb {
    Vector<Property>        all_properties();
    size_t                  all_properties_count();
    
    Atom                    atom(Property);
    
    bool                    exists(Property);
    bool                    exists_property(uint64_t);
    
    
    String                  key(Property);
    AttrKind                kind(Property);
    
    Property                property(uint64_t);
    
    String                  type(Property);
    String                  uid(Property);
    String                  value(Property);
    
}
