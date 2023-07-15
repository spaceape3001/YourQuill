////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uField.hpp"
#include <mithril/field/FieldCDB.hpp>

namespace yq::mithril::update {
    UField&  UField::get(Field f)
    {
        static constexpr const size_t       kMinAlloc   = 2048uz;
        static std::vector<UField*>      s_lookup(kMinAlloc, nullptr);
        if(s_lookup.size() <= f.id)
            s_lookup.resize(f.id+kMinAlloc, nullptr);
            
        auto& u = s_lookup[f.id];
        if(!u)
            u   = new UField(f);
        return *u;
    }

    UField::UField(Field f) : field(f), id(f.id), key(cdb::key(f))
    {
    }
}

