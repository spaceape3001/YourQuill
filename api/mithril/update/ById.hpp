////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfo.hpp>
#include <mithril/id/Id.hpp>

namespace yq::mithril::update {

    /*! \brief Lookup comparison
    
        Used for maps, the lookup for an ID type of key.
    */
    struct ById {
        static Meta::id_t   id_for(const TypeInfo* t)
        {
            return t ? t->id() : 0;
        }
        
        bool    operator()(const TypeInfo* a, const TypeInfo* b) const
        {
            return id_for(a) < id_for(b);
        }
        
        template <IdType T>
        constexpr bool    operator()(T a, T b) const
        {
            return a.id < b.id;
        }
    };
    
}


