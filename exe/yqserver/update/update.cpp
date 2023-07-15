////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uClass.hpp"
#include "uField.hpp"
#include <mithril/class/ClassCDB.hpp>
#include <mithril/field/FieldCDB.hpp>

namespace yq::mithril::update {
    ////////////////////////////////////////////////////////////////////////////////

    UClass&  UClass::get(Class c)
    {
        static constexpr const size_t       kMinAlloc   = 1024uz;
        static std::vector<UClass*>      s_lookup(kMinAlloc, nullptr);
        if(s_lookup.size() <= c.id)
            s_lookup.resize(c.id+kMinAlloc, nullptr);
        
        auto& u = s_lookup[c.id];
        if(!u)
            u   = new UClass(c);
        return *u;
    }

    UClass::UClass(Class c) : class_(c), id(c.id), key(cdb::key(c)) 
    {
    }
    
    ////////////////////////////////////////////////////////////////////////////////

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

    ////////////////////////////////////////////////////////////////////////////////
}

